#include "x64emu/alu.hpp"
#include "x64emu/cpu.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    x64emu::CpuState cpu;
    const auto largest_signed = static_cast<std::uint64_t>(
        std::numeric_limits<std::int64_t>::max());
    const auto result = x64emu::add64(cpu, largest_signed, 1);
    cpu.write(x64emu::Gpr::rax, result);

    std::cout << "Milestone 1: x86-64 CPU state and flag semantics\n"
              << "RAX    = 0x" << std::hex << std::setw(16)
              << std::setfill('0') << cpu.read(x64emu::Gpr::rax) << '\n'
              << "RFLAGS = 0x" << std::setw(16) << cpu.rflags() << '\n'
              << "OF     = " << cpu.flag(x64emu::Flag::overflow) << '\n';
}
