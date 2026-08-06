# x86-64 Emulator

A from-scratch x86-64 emulator and debugger written in modern C++. The goal is
to turn classroom familiarity with assembly into a tested, inspectable systems
project covering instruction semantics, machine-code decoding, ELF loading,
virtual memory, system calls, debugging, SIMD, and eventually JIT compilation.

## Current status

**Milestone 1 — CPU state and integer flags (in progress)**

- 16 64-bit general-purpose registers, `RIP`, and `RFLAGS`
- Correct `CF`, `PF`, `AF`, `ZF`, `SF`, and `OF` behavior for 64-bit add/subtract
- Dependency-free unit-test executable
- C++20 library and demonstration CLI

## Build in WSL

```bash
git clone https://github.com/TorinChacko/x86-64-emulator.git
cd x86-64-emulator
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
./build/x64emu_cli
```

If Ninja is unavailable, omit `-G Ninja`.

## Roadmap

- [ ] **CPU core:** subregisters, arithmetic widths, shifts, condition codes
- [ ] **Decoder:** legacy/REX prefixes, opcode maps, ModR/M, SIB, immediates
- [ ] **Execution:** data movement, ALU, branches, calls, stack operations
- [ ] **Memory:** little-endian reads, mapped regions, permissions, faults
- [ ] **ELF:** load static ELF64 executables and establish process state
- [ ] **Linux ABI:** System V calling convention and a focused syscall layer
- [ ] **Debugger:** stepping, breakpoints, watchpoints, register/memory views
- [ ] **Validation:** native differential tests, decoder fuzzing, benchmarks
- [ ] **Extensions:** SSE/AVX, atomics, threads, paging, and a JIT backend

## Learning method

Each milestone follows the same loop:

1. Read the relevant architectural or ABI specification.
2. Predict behavior with small handwritten assembly experiments.
3. Implement the behavior in the emulator.
4. Compare emulator state against native hardware.
5. Document edge cases and design decisions.

The project intentionally begins with flags: they expose the difference between
bit patterns, unsigned arithmetic, signed interpretation, and architectural
side effects—concepts that recur throughout x86-64.

## Documentation

- [Architecture notes](docs/architecture.md)
- [Learning log](docs/learning-log.md)

## License

MIT
