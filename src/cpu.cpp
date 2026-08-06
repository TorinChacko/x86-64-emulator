#include "x64emu/cpu.hpp"

namespace x64emu {

std::uint64_t CpuState::read(const Gpr reg) const {
    return registers_.at(static_cast<std::size_t>(reg));
}

void CpuState::write(const Gpr reg, const std::uint64_t value) {
    registers_.at(static_cast<std::size_t>(reg)) = value;
}

bool CpuState::flag(const Flag flag) const {
    return (rflags_ & static_cast<std::uint64_t>(flag)) != 0;
}

void CpuState::set_flag(const Flag flag, const bool value) {
    const auto mask = static_cast<std::uint64_t>(flag);
    if (value) {
        rflags_ |= mask;
    } else {
        rflags_ &= ~mask;
    }
    rflags_ |= 1ULL << 1;
}

} // namespace x64emu
