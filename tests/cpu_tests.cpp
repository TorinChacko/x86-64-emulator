#include "x64emu/alu.hpp"
#include "x64emu/cpu.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <string_view>

namespace {

int failures = 0;

void expect(const bool condition, const std::string_view description) {
    if (!condition) {
        std::cerr << "FAIL: " << description << '\n';
        ++failures;
    }
}

void test_cpu_state() {
    x64emu::CpuState cpu;
    cpu.write(x64emu::Gpr::r15, 0xfeedfaceULL);
    expect(cpu.read(x64emu::Gpr::r15) == 0xfeedfaceULL,
           "general-purpose registers round-trip");
    expect(cpu.rflags() == 0x2, "reserved RFLAGS bit 1 starts set");
}

void test_add_flags() {
    x64emu::CpuState cpu;
    const auto wrapped = x64emu::add64(
        cpu, std::numeric_limits<std::uint64_t>::max(), 1);
    expect(wrapped == 0, "unsigned addition wraps at 64 bits");
    expect(cpu.flag(x64emu::Flag::carry), "add records unsigned carry");
    expect(cpu.flag(x64emu::Flag::zero), "add records a zero result");
    expect(!cpu.flag(x64emu::Flag::overflow),
           "unsigned wrap need not be signed overflow");
    expect(cpu.flag(x64emu::Flag::parity), "zero has even low-byte parity");

    const auto signed_max = static_cast<std::uint64_t>(
        std::numeric_limits<std::int64_t>::max());
    const auto signed_wrap = x64emu::add64(cpu, signed_max, 1);
    expect(signed_wrap == (1ULL << 63), "signed maximum wraps to minimum");
    expect(cpu.flag(x64emu::Flag::overflow), "add records signed overflow");
    expect(cpu.flag(x64emu::Flag::sign), "wrapped result has its sign bit set");
}

void test_sub_flags() {
    x64emu::CpuState cpu;
    const auto result = x64emu::sub64(cpu, 0, 1);
    expect(result == std::numeric_limits<std::uint64_t>::max(),
           "subtraction wraps at 64 bits");
    expect(cpu.flag(x64emu::Flag::carry),
           "sub records borrow in the carry flag");
    expect(cpu.flag(x64emu::Flag::sign), "sub records a negative result");

    const auto signed_min = 1ULL << 63;
    static_cast<void>(x64emu::sub64(cpu, signed_min, 1));
    expect(cpu.flag(x64emu::Flag::overflow), "sub records signed overflow");
}

} // namespace

int main() {
    test_cpu_state();
    test_add_flags();
    test_sub_flags();

    if (failures == 0) {
        std::cout << "All CPU tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
