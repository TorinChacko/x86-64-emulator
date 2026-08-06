# Architecture notes

## Design principles

- Keep architectural state separate from instruction decoding and execution.
- Represent integer state with unsigned fixed-width types; signedness belongs
  to an operation's interpretation, not to a register.
- Test edge cases directly against documented x86-64 behavior.
- Add abstraction only after at least two real instruction forms require it.

## Initial component model

```text
raw bytes -> decoder -> instruction -> executor -> CPU state
                                      -> memory
```

The first milestone implements the CPU state and a small ALU independently of
the decoder. This makes flag semantics testable before variable-length x86
instruction encoding is introduced.

## RFLAGS observations

- `CF` describes unsigned carry for addition and unsigned borrow for subtraction.
- `OF` describes whether the signed mathematical result is unrepresentable.
- `SF` copies the result's most-significant bit.
- `ZF` records whether the width-truncated result is zero.
- `PF` is even parity of only the low result byte.
- `AF` records carry/borrow across bit 3 and is mostly a historical artifact.
