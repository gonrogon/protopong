#===========#
# Processor #
#===========#

set(PONG_CPU_ARM   0)
set(PONG_CPU_MIPS  0)
set(PONG_CPU_PPC   0)
set(PONG_CPU_RISCV 0)
set(PONG_CPU_X86   0)

message(STATUS "Detecting CPU...")
string(TOUPPER "${CMAKE_SYSTEM_PROCESSOR}" M_TARGET_ARCH)
# Detect x86 (32-bit o 64-bit)
if(M_TARGET_ARCH MATCHES "^(X86|AMD64|I[3-6]86)")
    set(PONG_CPU_X86 1)
    message(STATUS ">> X86")
    # Detect ARM (ARM o ARM64/AArch64)
elseif(M_TARGET_ARCH MATCHES "^(ARM|AARCH)")
    set(PONG_CPU_ARM 1)
    message(STATUS ">> ARM")
    # Detect MIPS
elseif(M_TARGET_ARCH MATCHES "^(MIPS)")
    set(PONG_CPU_MIPS 1)
    message(STATUS ">> MIPS")
    # Detect PowerPC (PPC)
elseif(M_TARGET_ARCH MATCHES "^(PPC|POWERPC)")
    set(PONG_CPU_PPC 1)
    message(STATUS ">> PPC")
    # Detect RISC-V
elseif(M_TARGET_ARCH MATCHES "^(RISCV)")
    set(PONG_CPU_RISCV 1)
    message(STATUS ">> RISC-V")
else()
    message(WARNING ">> CPU architecture not recognized or supported: ${CMAKE_SYSTEM_PROCESSOR}.")
endif()

#==========#
# Platform #
#==========#

set(PONG_PLATFORM_ANDROID 0)
set(PONG_PLATFORM_BSD     0)
set(PONG_PLATFORM_IOS     0)
set(PONG_PLATFORM_LINUX   0)
set(PONG_PLATFORM_OSX     0)
set(PONG_PLATFORM_WINDOWS 0)
set(PONG_PLATFORM_WINRT   0)
set(PONG_PLATFORM_RPI     0)

message(STATUS "Detecting platform...")
# Detect Android.
if(ANDROID)
    set(PONG_PLATFORM_ANDROID 1)
    message(STATUS ">> Android")
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(PONG_PLATFORM_IOS 1)
    message(STATUS ">> iOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
    set(PONG_PLATFORM_OSX 1)
    message(STATUS ">> macOS (OSX)")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # Detect Raspberry Pi by the CPU model and file existence
    if(PONG_CPU_ARM AND EXISTS "/etc/rpi-issue")
        set(PONG_PLATFORM_RPI 1)
        message(STATUS ">> RaspberryPi")
    else()
        set(PONG_PLATFORM_LINUX 1)
        message(STATUS ">> Linux")
    endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "^(FreeBSD|OpenBSD|NetBSD|DragonFly)")
    set(PONG_PLATFORM_BSD 1)
    message(STATUS ">> BSD")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    if(CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")  # WinRT/UWP
        set(PONG_PLATFORM_WINRT 1)
        message(STATUS ">> WindowsRT")
    else()
        set(PONG_PLATFORM_WINDOWS 1)
        message(STATUS ">> Windows")
    endif()
else()
    message(WARNING ">> Unknown platform or not supported: ${CMAKE_SYSTEM_NAME}")
endif()

#==============#
# Architecture #
#==============#

set(PONG_ARCH_32BIT 0)
set(PONG_ARCH_64BIT 0)

message(STATUS "Detecting architecture...")
if(NOT CMAKE_SIZEOF_VOID_P)
    message(FATAL_ERROR "CMAKE_SIZEOF_VOID_P not defined.")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PONG_ARCH_64BIT 1)
    message(STATUS ">> 64-bit")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PONG_ARCH_32BIT 1)
    message(STATUS ">> 32-bit")
else()
    message(WARNING ">> Unknown pointer size (sizeof(void*) = ${CMAKE_SIZEOF_VOID_P})")
endif()

#============#
# Endianness #
#============#

set(PONG_ENDIAN_BIG    0)
set(PONG_ENDIAN_LITTLE 0)

message(STATUS "Detecting endianness...")
if(CMAKE_CXX_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    set(PONG_ENDIAN_BIG 1)
    message(STATUS ">> Big Endian")
else()
    set(PONG_ENDIAN_LITTLE 1)
    message(STATUS ">> Little Endian")
endif()

#==========#
# Compiler #
#==========#

set(PONG_COMPILER_CLANG           0)
set(PONG_COMPILER_GCC             0)
set(PONG_COMPILER_MSVC            0)
set(PONG_COMPILER_MSVC_COMPATIBLE 0)

message(STATUS "Detecting compiler...")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(PONG_COMPILER_CLANG 1)
    message(STATUS ">> Clang")
    # Clang could be compatible with MSVC
    if(CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
        set(PONG_COMPILER_MSVC_COMPATIBLE 1)
        message(STATUS ">> MSVC compatible")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(PONG_COMPILER_GCC 1)
    message (STATUS ">> GNU")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(PONG_COMPILER_MSVC 1)
    set(PONG_COMPILER_MSVC_COMPATIBLE 1)
    message(STATUS ">> MSVC")
    message(STATUS ">> MSVC compatible")
endif()
