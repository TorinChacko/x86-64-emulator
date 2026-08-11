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

## Reading log: foundations for executing machine code

### 64-bit and smaller register views

The general-purpose registers hold 64-bit bit patterns. Names such as `RAX`,
`EAX`, `AX`, and `AL` are overlapping views of the same architectural
register, not four independent storage locations:

| Name | Width | Effect of a write |
| --- | ---: | --- |
| `RAX` | 64 bits | Replaces all 64 bits. |
| `EAX` | 32 bits | Replaces bits 0-31 and clears bits 32-63. |
| `AX` | 16 bits | Replaces bits 0-15 and preserves the upper 48 bits. |
| `AL` | 8 bits | Replaces bits 0-7 and preserves the upper 56 bits. |
| `AH` | 8 bits | Replaces bits 8-15, but is unavailable in an instruction that uses a REX prefix. |

The zero-extension caused by a 32-bit write is especially important. For
example, `mov eax, 1` makes the complete `RAX` value equal to 1. Arithmetic
must also use the selected operand width: the result is truncated to that
width, and `CF`, `OF`, `SF`, `ZF`, `AF`, and `PF` are calculated from that
width's result. A 32-bit arithmetic result written to a general-purpose
register then receives the normal 32-bit-write zero extension.

`RSI` follows the same naming pattern: `RSI`, `ESI`, `SI`, and `SIL`. The name
means source index because string instructions use it implicitly, but ordinary
instructions can use it as a general-purpose register. Under the System V
AMD64 ABI, `RSI` also carries the second integer or pointer argument.

`RAX` has several conventional and implicit roles in addition to being a
general-purpose register. It is the accumulator used by some instruction
forms, it holds integer return values in the System V AMD64 ABI, and Linux
uses it for the system-call number and return value. Those are instruction or
ABI conventions; the register is not permanently restricted to those jobs.

### Status flags and conditional control flow

Arithmetic stores both a result and facts about that result in `RFLAGS`:

- `CF` reports unsigned carry from addition or unsigned borrow from subtraction.
- `OF` reports signed overflow at the selected operand width.
- `ZF` is set when the width-truncated result is zero.
- `SF` copies the result's most-significant bit.
- `PF` reports even parity in the low result byte only.
- `AF` reports a carry or borrow across bit 3.

Unsigned and signed overflow are different interpretations of the same bits.
For 64-bit addition, `0xffffffffffffffff + 1` wraps to zero and sets `CF`, but
does not set `OF`: interpreted as signed values, that is `-1 + 1`. Conversely,
`0x7fffffffffffffff + 1` sets `OF` but not `CF`: the unsigned sum still fits in
64 bits, while the signed positive result does not.

Conditional jumps read flags; they do not consume a C++-style boolean saved by
the previous instruction. `JNZ`/`JNE` jumps when `ZF == 0`, so it naturally
continues a loop after `sub rcx, 1` until `RCX` becomes zero.

Important jump encodings include:

- `EB rel8`: short unconditional `JMP`.
- `E9 rel32`: near relative `JMP` in 64-bit mode.
- `FF /4`: near indirect `JMP` through a register or memory operand.
- `70`-`7F rel8`: short conditional jumps; `75` is `JNZ rel8`.
- `0F 80`-`0F 8F rel32`: near conditional jumps.

A relative displacement is signed and is added to the address immediately
after the jump instruction. Therefore, in `75 F7`, the byte `F7` is `-9`, and
the target is `next_rip - 9`. Correct decoding must advance `RIP` by the full
instruction length before applying that displacement.

### Stack and ABI notes

`RSP` points at the current top of the stack. In the usual x86-64 stack model,
the stack grows toward lower addresses. A 64-bit `PUSH` first subtracts 8 from
`RSP` and stores the value; `POP` loads the value and then adds 8. `CALL` saves
the return address on the stack, and `RET` loads that address into `RIP`.

The CPU supplies these instruction semantics, while an ABI supplies additional
rules such as argument locations, which registers a function must preserve,
and stack alignment. Stack canaries are created and checked by the compiler and
runtime; they are not a CPU flag or a built-in property of `PUSH` and `POP`.
They belong after memory, stack instructions, process initialization, and ABI
support exist in the emulator.

### ELF notes

ELF is the executable and object-file container that a loader interprets; the
CPU executes the mapped instruction bytes, not the ELF structure itself. An
ELF loader will eventually need to validate the header, inspect program
headers, map loadable segments, apply their permissions, establish initial
process state, and set `RIP` to the entry point.

Several unrelated fields are called flags, so their scope matters:

- Program-header `p_flags` include `PF_R`, `PF_W`, and `PF_X`, describing
  whether a loaded segment should be readable, writable, or executable.
- Section-header `sh_flags` describe properties such as writable, allocated,
  or executable section contents. Sections mainly serve linking and analysis;
  program headers guide process loading.
- ELF-header `e_flags` contains architecture-specific file flags. It is not the
  CPU's `RFLAGS` register.

ELF32 versus ELF64 describes the layout of ELF metadata, while 32-bit versus
64-bit register operands describes instruction behavior. These concepts meet
when a loader chooses the execution environment, but they are not
interchangeable.

## Milestone 2: raw-byte execution checklist

The immediate target is to execute raw machine-code bytes before attempting
ELF loading:

```asm
mov rax, 0
mov rcx, 10

loop:
    add rax, rcx
    sub rcx, 1
    jnz loop

hlt
```

Expected final state: `RAX = 55`, `RCX = 0`, and `ZF = 1`.

Program bytes:

```text
48 B8 00 00 00 00 00 00 00 00
48 B9 0A 00 00 00 00 00 00 00
48 01 C8
48 83 E9 01
75 F7
F4
```

Implementation order:

- [ ] Add 64-, 32-, 16-, and 8-bit register reads and writes.
- [ ] Generalize addition and subtraction flags across all four widths.
- [ ] Define decoded instructions and typed operands separately from execution.
- [ ] Decode only the required `MOV`, `ADD`, `SUB`, `JNZ`, and `HLT` forms.
- [ ] Initially accept only register-direct (`mod == 0b11`) ModR/M operands.
- [ ] Advance to `next_rip` before executing a relative branch.
- [ ] Add a stepping loop and optional instruction/register trace.
- [ ] Test truncation, invalid forms, register encodings, sign extension, branch
  direction, taken and untaken branches, and fetch boundaries.
- [ ] Add an integration test for the complete sum-to-55 program.

Only after this raw-byte program works should the decoder grow effective-address
support through ModR/M and SIB. That provides the bridge to memory, stack
instructions, and eventually ELF loading.
