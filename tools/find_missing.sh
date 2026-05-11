#!/bin/bash

# Script to find missing LASX instructions by comparing lasx_instructions.md with implemented instructions

# Extract all instruction mnemonics from lasx_instructions.md
instructions=$(grep -E "^\| \w+ " lasx_instructions.md | awk '{print $2}')

total=$(echo "$instructions" | wc -l)
echo "Total instructions in ISA: $total"
echo

missing=()

# Check each instruction
for instr in $instructions; do
    # Search for the instruction handler in all .c files
    found=$(grep -r "do_lasx_emu_$instr" --include="*.c" . | wc -l)
    if [ "$found" -eq 0 ]; then
        # Also check for special cases where instruction name might be split
        found_alt=$(grep -r "$instr" --include="*.h" . | grep -E "(void|define)" | wc -l)
        if [ "$found_alt" -eq 0 ]; then
            missing+=("$instr")
        fi
    fi
done

# Sort missing instructions
IFS=$'\n' missing=($(sort <<<"${missing[*]}"))
unset IFS

missing_count=${#missing[@]}
echo "Missing instructions: $missing_count"
echo
echo "List of missing instructions:"
echo "-------------------------"
for i in "${missing[@]}"; do
    echo "$i"
done
echo
echo "Total: $missing_count / $total instructions missing"