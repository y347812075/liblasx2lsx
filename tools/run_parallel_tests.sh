#!/bin/bash

MAX_JOBS=${1:-4}
TOTAL_TESTS=${2:-20}

echo "Running $TOTAL_TESTS tests with up to $MAX_JOBS parallel jobs"

mkdir -p /tmp/lasx_test_logs

SEEDS=()
for i in $(seq 1 $TOTAL_TESTS); do
    SEEDS+=($((RANDOM * 1000 + i)))
done

FAILED_TESTS=()

run_single_test() {
    local seed=$1
    local log_file="/tmp/lasx_test_logs/seed_$seed.log"
    
    echo "[seed=$seed] Starting..."
    if make test-random-compare SEED=$seed > "$log_file" 2>&1; then
        if grep -q "Random tests compare: .* 0 failed" "$log_file"; then
            echo "[seed=$seed] PASSED"
            rm -f "$log_file"
            return 0
        fi
    fi
    echo "[seed=$seed] FAILED"
    return 1
}

export -f run_single_test

run_with_limit() {
    local max_jobs=$1
    shift
    local seeds=("$@")
    
    local pids=()
    local idx=0
    
    while [ $idx -lt ${#seeds[@]} ]; do
        # Start up to max_jobs
        while [ $idx -lt ${#seeds[@]} ] && [ ${#pids[@]} -lt $max_jobs ]; do
            run_single_test "${seeds[$idx]}" &
            pids+=($!)
            idx=$((idx + 1))
        done
        
        # Wait for at least one to finish
        if [ ${#pids[@]} -ge $max_jobs ]; then
            for pid in "${pids[@]}"; do
                wait $pid 2>/dev/null || true
            done
            pids=()
        fi
    done
    
    # Wait for remaining
    for pid in "${pids[@]}"; do
        wait $pid 2>/dev/null || true
    done
}

if [ "$MAX_JOBS" -eq 1 ]; then
    for seed in "${SEEDS[@]}"; do
        run_single_test $seed || FAILED_TESTS+=($seed)
    done
else
    run_with_limit $MAX_JOBS "${SEEDS[@]}"
fi

echo ""
echo "========================================"

failed_count=0

for seed in "${SEEDS[@]}"; do
    log_file="/tmp/lasx_test_logs/seed_$seed.log"
    if [ -f "$log_file" ]; then
        if grep -q "FAIL:" "$log_file"; then
            FAILED_TESTS+=($seed)
            failed_count=$((failed_count + 1))
        fi
    fi
done

if [ $failed_count -eq 0 ]; then
    echo "All $TOTAL_TESTS tests passed!"
    rm -rf /tmp/lasx_test_logs
    exit 0
else
    echo ""
    echo "$failed_count test run(s) failed:"
    for seed in "${FAILED_TESTS[@]}"; do
        log_file="/tmp/lasx_test_logs/seed_$seed.log"
        echo "  - SEED=$seed"
        if [ -f "$log_file" ]; then
            failed_instrs=$(grep "^FAIL:" "$log_file" | sed 's/FAIL: //')
            echo "    Failed instructions:"
            for instr in $failed_instrs; do
                echo "      make test-single TEST=$instr SEED=$seed"
                echo "      make test-single-native TEST=$instr SEED=$seed"
            done
        fi
    done
    echo ""
    echo "========================================"
    echo "Quick retest all:"
    for seed in "${FAILED_TESTS[@]}"; do
        echo "  make test-random-compare SEED=$seed"
    done
    exit 1
fi