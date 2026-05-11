# Development Guide

## 1. Architecture Overview

liblasx2lsx intercepts LASX (LoongArch SIMD Extension) instructions at runtime on systems without native LASX support. It works by:

1. **LD_PRELOAD** — a shared library loaded before all others
2. **SIGILL capture** — LASX instructions not supported by hardware raise SIGILL
3. **Dual translation paths** — pure C emulation (fallback) or JIT compilation to LSX (128-bit)

```
Application (uses LASX instructions)
    │ LD_PRELOAD=./liblasx2lsx.so
    ▼
┌─────────────────────────────────────┐
│ sigill_hook.c                       │
│  register_sigill_handler()          │
│  → sigaction(SIGILL)                │
└─────────┬───────────────────────────┘
          │ LASX instr → SIGILL
          ▼
┌─────────────────────────────────────┐
│ sigill_handler() fallback chain:    │
│                                     │
│ 1. lasx_emu_create_interpret_block  │  JIT batch (block/loop/usedef)
│ 2. lasx_emu_create_interpret        │  JIT single instruction
│ 3. do_lasx_emu()                    │  Pure C emulation
└─────────────────────────────────────┘
```

## 2. Core Data Model

### Vector Register Layout

A LASX register (XR) is 256 bits = 4 × uint64_t slots (little-endian):

```
D[0] = B[0..7]    → W[0] = B[0..3], W[1] = B[4..7]
D[1] = B[8..15]   → W[2] = B[8..11], W[3] = B[12..15]
D[2] = B[16..23]  → W[4] = B[16..19], W[5] = B[20..23]
D[3] = B[24..31]  → W[6] = B[24..27], W[7] = B[28..31]
```

### Thread-Local State

Each thread has a `thread_data_t`:

```c
typedef struct {
    uint64_t gpr[32];                     // GPR snapshot
    uint64_t data[32][4];                 // XR[0..31], 4 uint64_t each
    uint64_t data_vr[32][2];             // VR high-half storage (for JIT)
    FILE *log_file;                       // JIT log
    uint32_t instr_count;                 // instruction counter
    uint64_t vregs_fcc;                   // floating-point condition codes
} thread_data_t;
```

Access via `thread_data_get()` — lazy pthread TLS init.

### VREG Read/Write Functions

Defined in `lasx_emu_private.h`. Each operates on a specific element width:

| Function | Width | Granularity |
|----------|-------|-------------|
| `vreg_read_64` / `vreg_write_64` | 64-bit | Per dword slot (0..3) |
| `vreg_read_32` / `vreg_write_32` | 32-bit | Per word slot (0..7) |
| `vreg_read_16` / `vreg_write_16` | 16-bit | Per halfword slot (0..15) |
| `vreg_read_8` / `vreg_write_8` | 8-bit | Per byte slot (0..31) |
| `vreg_read_128` / `vreg_write_128` | 128-bit | Per 128-bit pair (0..1) |

**Critical:** Write granularity must match the element width. Writing 64-bit when the output is 16-bit elements will clobber adjacent values.

## 3. SIGILL Handler

### Initialization

`register_sigill_handler()` — a `__attribute__((constructor))` that runs on `LD_PRELOAD`:

1. Skips if not loaded via `LD_PRELOAD`
2. Reads `DISABLE_LSX_INTRINSICS` env var
3. Reads `LASX_PERF_STATS=1` for performance counters
4. Reads `LIBLASX2LSX_INTERPRET_MODE` for JIT mode selection:
   - `block` — batch consecutive XV instructions
   - `loop` — detect and JIT whole loops
   - `usedef` — use-def analysis for VR remapping
   - Default: `usedef | loop | frag` combined
5. Calls `lasx_init_interpret()` to mmap JIT code buffer at `0x60000000`
6. Registers `sigill_handler` via `sigaction(SIGILL, SA_SIGINFO)`

### Handler Fallback Chain

```c
void sigill_handler(int sig, siginfo_t *info, void *context) {
    // 1. Try fragment JIT
    if (lasx_emu_create_interpret_fragment(uc)) { return; }
    // 2. Try block JIT (loop/usedef/plain block)
    if (lasx_emu_create_interpret_block(uc)) { return; }
    // 3. Try single-instruction JIT
    if (lasx_emu_create_interpret(uc, instr)) { return; }
    // 4. Fallback: pure C emulation
    int ret = do_lasx_emu(uc, instr);
    if (ret) {
        uc->uc_mcontext.__pc += 4;  // advance past the instruction
    } else {
        // Unknown instruction — restore default SIGILL, let process crash
        signal(SIGILL, SIG_DFL);
    }
}
```

Each JIT step **modifies the original code in-place** by replacing the target instruction with `jiscr1` (jump to interpreter scratch register). Subsequent executions hit the JIT code directly, not the SIGILL handler.

### Environment Variables

| Variable | Effect |
|----------|--------|
| `LD_PRELOAD=./liblasx2lsx.so` | Load the library |
| `LIBLASX2LSX_INTERPRET_MODE=block` | Block JIT only |
| `LIBLASX2LSX_INTERPRET_MODE=usedef` | Use-def JIT |
| `LIBLASX2LSX_DEBUG=1` | Enable debug logging (`tdlog`) |
| `LASX_PERF_STATS=1` | Per-instruction performance counters |
| `LASX_PROFILE=1` | JIT profiling (requires profile build) |

## 4. Pure Emulation (do_lasx_emu)

### Dispatch Table Structure

The main dispatcher `do_lasx_emu()` uses 13 sequential `switch` statements organized by opcode bit fields. The rule:

```
shift = 32 - OP_number
```

| Switch shift | OP prefix | Example cases |
|-------------|-----------|---------------|
| `instr >> 18` | OP14 | xvpermi, xvldi, xvandi_b, xvbitseli_b, xvshuf4i_* |
| `instr >> 20` | OP12 | xvfmadd, xvfnmsub, xvfcmp_cond_s/d |
| `instr >> 15` | OP17 | **Bulk**: arith, logic, cmp, float, mul/div, shift, extadd, bitmanip |
| `instr >> 8` | OP24 | xvseteqz, xvsetnez, xvsetanyeqz, xvsetallnez |
| `instr >> 10` | OP22 | xvfsqrt, xvfrecip, xvfrecipe, xvfclass, xvfcvt, xvffint, xvftint |
| Various | OP21-O9 | xvrepl128vei, xvinsgr2vr, xvbitseti, xvslli, xvsrani, xvld/xvst |

**Wrong shift = silent dispatch failure.** An instruction with the wrong shift will fall through to the next switch and be misinterpreted or unrecognized.

### Emulation Pattern

Every instruction follows the same pattern:

```c
case OP17_XVADD_B:
    perf_inc(P_XVADD_B);
    do_lasx_emu_xvadd_b(uc, instr);
    return 1;
```

Each `do_lasx_emu_*` function:
1. Decodes register operands from `instr`
2. Reads source VREGs via `vreg_read_*`
3. Computes result per element
4. Writes result via `vreg_write_*`

### Key Constraint: Read-Modify-Write (RMW)

When `xd == xj` or `xd == xk`, iterating while reading and writing xd corrupts subsequent iterations.

**Wrong:**
```c
for (int i = 0; i < 4; i++) {
    uint64_t src = vreg_read_64(uc, td, xj, i);
    uint64_t dst = vreg_read_64(uc, td, xd, i);  // xd == xj → reads modified value!
    dst = compute(dst, src);
    vreg_write_64(uc, td, xd, i, dst);
}
```

**Correct (per-slot atomic):**
```c
for (int slot = 0; slot < 4; slot++) {
    uint64_t src_j = vreg_read_64(uc, td, xj, slot);
    uint64_t src_k = vreg_read_64(uc, td, xk, slot);
    uint64_t dst = 0;
    // process all elements in this slot
    dst = compute(...);
    vreg_write_64(uc, td, xd, slot, dst);
}
```

## 5. JIT Translation

### Three Strategies

The JIT translates consecutive LASX instructions into LSX (128-bit) equivalents. Each LASX instruction becomes 1-2 LSX instructions operating on the low and high 128-bit halves.

#### Block Mode

- Scans forward for consecutive XV instructions (`detect_xv_batch()`)
- Translates each as a simple 2×LSX sequence
- Replaces first instruction with `jiscr1` → JIT code
- Returns via `jiscr0`

#### Usedef Mode

Builds a use-def chain across the batch to eliminate redundant high-half loads/stores:

1. `build_block_usedef()` — linear scan of all XV instructions in batch
   - Tracks `first_use`, `first_write`, `last_use`, `last_write` per XR
   - Assigns temp VR to each XR that needs its high half live
   - Two passes: first allocates all active XRs, second allocates only written XRs
2. Emits prologue — saves original temp VR values, loads high halves
3. Translates each instruction with `temp_vr` mapping for high-half VR
4. Emits epilogue — stores modified high halves, restores temp VRs

Common bug: prologue re-saves the same physical VR when two XR intervals are assigned the same temp VR by the allocator. Fix: use a `saved_mask` bitmap.

#### Loop Mode

Detects backward branches and JITs the entire loop body. See Section 6.

### Interpret Pipeline Files

| File | Role |
|------|------|
| `lasx_interpret.c` | Entry point: `lasx_emu_create_interpret_block()`, `lasx_emu_create_interpret()`, batch detection, jiscr1 patching |
| `lasx_interpret_opt_gen.c` | Macro-generated translators: `GEN_OPT_3OP`, `GEN_OPT_2OP` produce `__gen_lasx_interpret_XXX_opt()` |
| `lasx_interpret_opt_gen_xvmap.c` | Special-case translators for complex instructions (xvld, xvst, vext2xv, xvpermi_q, xvfcmp_cond_s) |
| `lasx_interpret_opt_gen_usedef.c` | Usedef-aware translators with `int *temp_vr` parameter |
| `lasx_patterns_xvmap.c` | Loop pattern detection and xvmap-based loop JIT |

### The jiscr1/jiscr0 Mechanism

`jiscr1` (Jump to Interpreter Scratch Register 1) replaces the original instruction in the application's code. When execution reaches it:
1. Jumps to JIT code (address stored in scratch register)
2. JIT code executes the translated instruction sequence
3. `jiscr0` returns to the instruction after the original

The translation is permanent for the process lifetime. No further SIGILL for that instruction.

## 6. Loop Pattern Detection

### Backward Scan

When a SIGILL occurs in the middle of a loop, `detect_loop_range()` scans backward from the faulting instruction to find the loop start:

1. Scan backward to find the first instruction of the pattern
2. Scan forward for a backward conditional branch (loop back edge)
3. Returns `{loop_start, loop_end, loop_len}`

### xvmap

The xvmap mechanism assigns each used XR a pair of physical VRs (low + high) and generates LSX code for the entire loop body. Key steps:

1. `check_xvmap_feasibility()` — validates all loop instructions are translatable
2. `generate_xvmap_loop()`:
   - Builds xvmap (assigns VR pairs)
   - Emits prologue: save callee-saved VRs, load high halves
   - Translates each LASX instruction to paired LSX
   - Emits epilogue: store high halves, restore VRs
3. Replaces loop entry with `jiscr1` → JIT code

### Optimization State

```c
static unsigned int *detected_loop_start = NULL;
static unsigned int *detected_loop_end = NULL;
void *detected_loop_jit_entry = NULL;
```

After optimization, `is_loop_already_optimized()` checks if the loop start is already a `jiscr1` instruction to avoid re-optimization.

## 7. Lagoon Assembler

### Overview

Lagoon is a complete LoongArch assembler implemented as a C library (`lagoon.c` + `lagoon.h`). It provides:

- Instruction encoding for the entire LA64 ISA (GPR, FPR, LSX, LASX)
- Label management with backpatching
- JIT buffer management with bounds checking

### Key API

```c
// Assembler state
lagoon_assembler_t as;
la_init_assembler(&as, buffer, capacity);

// Instruction emission (700+ functions)
la_add_d(&as, rd, rj, rk);         // GPR add
la_vadd_b(&as, vd, vj, vk);        // LSX vadd.b
la_xvfmadd_s(&as, xd, xj, xk, xa); // LASX xvfmadd.s

// Label management
la_label(&as, &label);
la_b(&as, &label);                  // forward branch
la_bind(&as, &label);              // backpatch target

// Buffer management
int n = la_get_inst_count(&as);
void *cursor = la_get_cursor(&as);
```

### emit32()

The core function: writes 4 bytes to the JIT buffer and advances the cursor. Every `la_*()` function ultimately calls `emit32()`.

## 8. Testing

### Test Suite Layout

```
tests/                  — Hand-written unit tests (21 files)
  test_xvadd.c          Basic arithmetic
  test_xvfbasic.c       Basic float
  test_xvfma.c          Float multiply-add
  test_xvblock42.c      42-instruction block JIT test
  test_xvblock38.c      38-instruction block JIT test
  test_xvmap_*.c        Loop JIT tests
  TESTS                 — Passes reliably
  TESTS_DEBUG           — Known issues (QEMU LASX differences)

random_test/            — Systematic random testing (363 instructions)
  src/                  707 .c test files (one per instruction + variants)
  result/               Expected output files
  test/                 Compiled test binaries
  random_data           Default 64-byte test input
```

### Test Targets

```bash
make test               # 703 tests via emulator (block mode)
make test-native        # 703 tests via native QEMU LASX
make test-loop          # block + loop mode
make test-usedef        # usedef mode
make test-single TEST=xvadd.b    # Single instruction test
make test-single TEST=xvadd.b SEED=12345  # With random seed
```

### Random Test Framework

Each test file:
1. Loads 64 bytes of random data into two 256-bit vectors
2. Executes the LASX instruction via inline assembly (`.word` directive)
3. Prints the 256-bit result as 4 hex `uint64_t`

Results are compared against QEMU native output (expected) or between emulator modes.

### Coverage Verification

```bash
python3 tools/verify_coverage.py     # Check switch structure coverage
python3 tools/check_lasx_opcodes.py  # Validate opcode definitions
```

## 9. Debugging Guide

### Debug Output (tdlog)

Set `LIBLASX2LSX_DEBUG=1` to enable thread-safe debug logging:

```c
tdlog("xr%d = 0x%016lx\n", reg, val);  // prints to per-thread log
```

### JIT Logging (LASX_PROFILE build)

With `lasx_profile.so` (profile build), each JIT translation is logged to `/tmp/lasx-jit-PID-TID.log`:

```bash
make profile            # Build liblasx2lsx_profile.so
LASX_PROFILE=1 LD_PRELOAD=./liblasx2lsx_profile.so ./program
```

Log format:
```
=== batch 6 instrs mode=usedef PC=0000007fdd0e219c ===
  0000007fdd0e219c: 77ec0441  xvpermi.q $xr1, $xr2, 1
  0000007fdd0e21a0: 75308442  xvfadd.s $xr2, $xr2, $xr1
```

These logs feed into `gen_usedef_tests.py` for regression testing.

### Performance Stats

```bash
LASX_PERF_STATS=1 LD_PRELOAD=./liblasx2lsx.so ./program
```

Prints top 20 most-executed instructions every ~1 second:
```
=== Performance Stats (top 20) ===
  xvadd.w    : 1234567 (23.4%)
  xvfmadd.s  : 987654  (18.7%)
  ...
```

### GDB Strategy

Since the library replaces instructions with `jiscr1`, standard GDB breakpoints on LASX instructions won't fire (the instruction has been overwritten). Strategies:

1. **Break before library load**: Set breakpoints before `LD_PRELOAD` takes effect
2. **HITRACE**: Use the HITRACE macro to annotate specific emulator functions
3. **Force pure emulation**: Set `LIBLASX2LSX_INTERPRET_MODE=one` to disable JIT entirely
4. **Per-instruction emit**: `la_insn_to_str()` prints the disassembly of emitted LSX instructions

### jit_compare

A standalone tool that compares block-mode and usedef-mode LSX code generation for a given instruction sequence:

```bash
make jit_compare
./jit_compare <hex_instr> [hex_instr...]
```

Outputs the LSX instructions emitted by each mode. Useful for debugging allocator decisions.

## 10. Known Bug Patterns

### Pattern 1: RMW (Read-Modify-Write)

**Symptom**: Wrong results when `xd == xj` or `xd == xk`.

**Root cause**: Iterating while reading from and writing to the same register.

**Fix**: Read all source operands per slot, compute, then write once per slot.

### Pattern 2: Wrong Write Granularity

**Symptom**: Adjacent elements get incorrect values.

**Root cause**: Using `vreg_write_64` when the output has 16-bit elements.

**Fix**: Match the write function to the element width:
- `_h_b` → `vreg_write_16`
- `_w_h` → `vreg_write_32`
- `_d_w` → `vreg_write_64`

### Pattern 3: Saturation Sign Loss

**Symptom**: Large negative saturated values become large positive values.

**Root cause**: Using an unsigned intermediate:

```c
// WRONG:
uint32_t c = (int32_t)saturated;  // sign lost

// RIGHT:
dst |= ((uint64_t)(int32_t)saturated << shift);
```

### Pattern 4: Wrong Operand Order

Some instructions have non-obvious operand order:

| Instruction | Semantics | Common Error |
|-------------|-----------|-------------|
| `xvandn.v` | `rd = rk & ~rj` | `rd = rj & ~rk` |
| `xvorn.v` | `rd = rk \| ~rj` | `rd = rj \| ~rk` |

### Pattern 5: vext2xv Source Type

`vext2xv` reads from a **vector register** (XR), not from GPR. All 12 variants:

| Instruction | Source → Dest | Extension |
|-------------|--------------|-----------|
| `vext2xv.h.b` | XR byte → XR half | Signed |
| `vext2xv.hu.bu` | XR byte → XR half | Unsigned |
| `vext2xv.w.h` | XR half → XR word | Signed |
| ... | | |

### Pattern 6: Narrowing Shift Storage Layout

For `xvsrln.b.h`, output bytes are stored non-contiguously:
- B[0..7] → D0
- B[8..15] → D1 (zeroed)
- B[16..23] → D2
- B[24..31] → D3 (zeroed)

### Pattern 7: Prologue Overwrite (Usedef)

**Symptom**: Usedef mode produces wrong results, block mode correct.

**Root cause**: Allocator assigns the same temp VR to two XRs. Prologue saves each separately, but the second save overwrites the first. Epilogue restores the wrong value.

**Fix**: Use `saved_mask` bitmap to deduplicate prologue saves and epilogue restores.

### Pattern 8: Interleaved Element Indexing

For `xvaddwev.h.b` (even elements):
- Output `H[i]` reads from `B[2i]` (byte at even position)
- Index calculation: `byte_idx = i * 2`

For `xvaddwod.h.b` (odd elements):
- Output `H[i]` reads from `B[2i+1]` (byte at odd position)
- Index calculation: `byte_idx = i * 2 + 1`

## 11. Future Improvements

### JIT Optimization

- **Usedef sub-interval allocation**: Currently the allocator either gives a temp VR for the full block or falls back to per-instruction. Sub-interval allocation (reusing VRs across non-overlapping XR lifetimes) would reduce register pressure.
- **Loop-aware usedef**: Combine loop detection with usedef analysis for more efficient loop JIT.
- **Fragment JIT**: The fragment mode exists but is incomplete (`lasx_emu_create_interpret_fragment` always returns false).

### Emulation Completeness

- **Remaining instructions**: A small number of LASX instructions (primarily xvshuf4i_d variants) still lack emulator implementations. Run `tools/find_missing_instructions.py` to check.
- **Float precision**: `xvfrecipe`, `xvfrsqrte`, `xvflogb` are excluded from random tests due to precision differences between QEMU and hardware. A hardware-based reference test suite is needed.

### Testing Infrastructure

- **Parallel random tests**: The `run_parallel_tests.sh` script works but lacks deterministic output ordering for CI integration.
- **Hardware CI**: Add 2K3000 or 3A6000 as a remote test target for native LASX verification.
- **Coverage-guided fuzzing**: AFL/libfuzzer integration to find edge cases in instruction emulation.

### Build System

- **Out-of-tree builds**: Currently all build artifacts pollute the source tree.
- **Meson/CMake migration**: The hand-written Makefile (671 lines) is growing unwieldy.
- **Windows cross-compile**: No support for cross-compiling to LA64 from Windows hosts.

### Code Quality

- **Reduce lagoon.c size**: At 32,474 lines, `lagoon.c` is one file. Split by instruction category.
- **Automated RMW checking**: A static analyzer pass to detect the RMW bug pattern would prevent common regressions.
- **Consistent naming**: Some functions use `lasx_emu_*`, others `do_lasx_emu_*`. Unify.
