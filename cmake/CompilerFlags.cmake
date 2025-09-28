set(PONG_CXX_FLAGS_RELWITHDEBINFO "")
set(PONG_CXX_FLAGS_DEBUG          "-fsanitize=address" "-fsanitize=undefined" "-g")
set(PONG_CXX_FLAGS_RELEASE        "")
# List with the common flags for GCC and Clang
set(PONG_CXX_COMMON_WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wnon-virtual-dtor
        -Wcast-align
        -Wchar-subscripts
        -Wpointer-arith
        -Wformat-security
        -Woverloaded-virtual
        -Wshadow
        -Wformat=2
        -Wundef
        -Wnull-dereference
        -Wdouble-promotion
        -Wimplicit-fallthrough=3
)

include(CheckCXXCompilerFlag)
# Set up the flags for GCC.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND PONG_CXX_FLAGS_WARN ${PONG_CXX_COMMON_WARNING_FLAGS})
    # This flag is useful as not returning from a non-void function is an error with MSVC, but it is not supported on
    # all GCC compiler versions.
    check_cxx_compiler_flag("-Werror=return-type" M_HAVE_ERROR_RETURN_TYPE)
    if (M_HAVE_ERROR_RETURN_TYPE)
        set(PONG_CXX_FLAGS_WARN -Werror=return-type)
    endif()
    # Set up the flags.
    set(PONG_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG}          ${PONG_CXX_FLAGS_WARN})
    set(PONG_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${PONG_CXX_FLAGS_WARN})
    set(PONG_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE}        ${PONG_CXX_FLAGS_WARN} -O2 -DNDEBUG)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND PONG_CXX_FLAGS_WARN /W4 /permissive-)
    # Set up the flags.
    set(PONG_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG}          ${PONG_CXX_FLAGS_WARN} /Od /RTC1)
    set(PONG_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${PONG_CXX_FLAGS_WARN} /O2 /DNDEBUG /Zi)
    set(PONG_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE}        ${PONG_CXX_FLAGS_WARN} /O2 /DNDEBUG)
endif()
