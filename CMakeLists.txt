cmake_minimum_required(VERSION 3.20)

project(x86_64_emulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(x64emu
    src/alu.cpp
    src/cpu.cpp
)

target_include_directories(x64emu PUBLIC include)
target_compile_options(x64emu PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Wconversion>
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->
)

add_executable(x64emu_cli src/main.cpp)
target_link_libraries(x64emu_cli PRIVATE x64emu)

include(CTest)
if(BUILD_TESTING)
    add_executable(x64emu_tests tests/cpu_tests.cpp)
    target_link_libraries(x64emu_tests PRIVATE x64emu)
    add_test(NAME cpu_tests COMMAND x64emu_tests)
endif()
