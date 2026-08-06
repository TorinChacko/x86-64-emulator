#include "x64emu/alu.hpp"

#include <bit>
#include <cstdint>
#include <limits>

namespace x64emu {
namespace {

bool even_parity(const std::uint64_t value) {
    return (std::popcount(static_cast<std::uint8_t>(value)) % 2) == 0;
}

void set_common_flags(CpuState& cpu, const std::uint64_t result) {
    cpu.set_flag(Flag::zero, result == 0);
    cpu.set_flag(Flag::sign, (result >> 63) != 0);
    cpu.set_flag(Flag::parity, even_parity(result));
}

} // namespace

std::uint64_t add64(
    CpuState& cpu,
    const std::uint64_t lhs,
    const std::uint64_t rhs) {
    const std::uint64_t result = lhs + rhs;
    const std::uint64_t sign_bit = 1ULL << 63;

    cpu.set_flag(Flag::carry, result < lhs);
    cpu.set_flag(Flag::auxiliary_carry, ((lhs ^ rhs ^ result) & 0x10) != 0);
    cpu.set_flag(Flag::overflow, ((~(lhs ^ rhs) & (lhs ^ result)) & sign_bit) != 0);
    set_common_flags(cpu, result);
    return result;
}

std::uint64_t sub64(
    CpuState& cpu,
    const std::uint64_t lhs,
    const std::uint64_t rhs) {
    const std::uint64_t result = lhs - rhs;
    const std::uint64_t sign_bit = 1ULL << 63;

    cpu.set_flag(Flag::carry, lhs < rhs);
    cpu.set_flag(Flag::auxiliary_carry, ((lhs ^ rhs ^ result) & 0x10) != 0);
    cpu.set_flag(Flag::overflow, (((lhs ^ rhs) & (lhs ^ result)) & sign_bit) != 0);
    set_common_flags(cpu, result);
    return result;
}

} // namespace x64emu
