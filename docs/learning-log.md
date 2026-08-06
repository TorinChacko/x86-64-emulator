# Learning log

Use this document as an engineering notebook. Portfolio reviewers value clear
reasoning and evidence at least as much as a long instruction list.

## Milestone 1: CPU state and flags

### Questions to answer

1. Why can `0xffffffffffffffff + 1` set `CF` without setting `OF`?
2. Why can `0x7fffffffffffffff + 1` set `OF` without setting `CF`?
3. Why does `SUB` use `CF` to represent a borrow?
4. Why is `PF` calculated from only the low byte?
5. Which instructions leave some status flags undefined rather than unchanged?

### Experiment record

For each experiment, record the assembly, expected flags, observed native
flags, emulator flags, and what explained any disagreement.
