#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace x64emu {

enum class Gpr : std::size_t {
    rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi,
    r8, r9, r10, r11, r12, r13, r14, r15,
};

enum class Flag : std::uint64_t {
    carry = 1ULL << 0,
    parity = 1ULL << 2,
    auxiliary_carry = 1ULL << 4,
    zero = 1ULL << 6,
    sign = 1ULL << 7,
    overflow = 1ULL << 11,
};

class CpuState {
public:
    [[nodiscard]] std::uint64_t read(Gpr reg) const;
    void write(Gpr reg, std::uint64_t value);

    [[nodiscard]] bool flag(Flag flag) const;
    void set_flag(Flag flag, bool value);

    [[nodiscard]] std::uint64_t rip() const { return rip_; }
    void set_rip(std::uint64_t value) { rip_ = value; }

    [[nodiscard]] std::uint64_t rflags() const { return rflags_; }

private:
    std::array<std::uint64_t, 16> registers_{};
    std::uint64_t rip_{};
    std::uint64_t rflags_{1ULL << 1}; // Architectural bit 1 is always set.
};

} // namespace x64emu
