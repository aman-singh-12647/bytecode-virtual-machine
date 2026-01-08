# Benchmarks & Performance
This document outlines basic performance metrics for the Bytecode Virtual Machine.

## Test Environment
- **OS**: Linux
- **CPU**: (Host CPU)
- **Compiler**: GCC (O0/O2 defaults from Makefile)

## Benchmark Results

Values collected using `/usr/bin/time` on compiled bytecode.

| Program | Description | Binary Size | Execution Time | Notes |
| :--- | :--- | :--- | :--- | :--- |
| `test1` | Circle Area (r=5) | 14 bytes | < 0.01s | Simple arithmetic, stack manipulation. |
| `test2` | Factorial (5!) | 73 bytes | < 0.01s | Loops, memory store/load. |
| `test3` | Fibonacci (8th) | 93 bytes | < 0.01s | Complex stack/memory usage. |
| `loop` | Tight Loop (10M) | 18 bytes | ~0.66s | Stress test. |

## Analysis

### Instruction Throughput
The `loop` benchmark performs a tight countdown loop 10,000,000 times.
The loop body consists of 4 instructions:
1. `PUSH 1`
2. `SUB`
3. `DUP`
4. `JNZ`

- **Total Instructions Executed**: ~40,000,000
- **Time**: 0.66s
- **Approximate MIPS**: ~60 MIPS

This indicates the VM has relatively low overhead for instruction dispatch, capable of executing tens of millions of instructions per second on standard hardware.

### Binary Size
Binary sizes are extremely compact, with functional programs (Factorial, Fibonacci) consuming less than 100 bytes. This confirms the efficiency of the bytecode format for small constrained environments.
