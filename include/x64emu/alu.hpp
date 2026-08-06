#pragma once

#include "x64emu/cpu.hpp"

#include <cstdint>

namespace x64emu {

[[nodiscard]] std::uint64_t add64(
    CpuState& cpu,
    std::uint64_t lhs,
    std::uint64_t rhs);

[[nodiscard]] std::uint64_t sub64(
    CpuState& cpu,
    std::uint64_t lhs,
    std::uint64_t rhs);

} // namespace x64emu
