set(PONG_DEBUG_FLAGS      "")
set(PONG_DEBUG_LINK_FLAGS "")
set(PONG_WARNING_FLAGS    "")
# List with the common flags for GCC and Clang
set(PONG_COMMON_WARNING_FLAGS
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
    -Wimplicit-fallthrough=3
    -Wno-unused-parameter
)

include(CheckCXXCompilerFlag)
# Set up the flags for GCC.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND PONG_WARNING_FLAGS ${PONG_COMMON_WARNING_FLAGS})
    # This flag is useful as not returning from a non-void function is an error with MSVC, but it is not supported on
    # all GCC compiler versions.
    check_cxx_compiler_flag(-Werror=return-type M_HAVE_ERROR_RETURN_TYPE)
    if (M_HAVE_ERROR_RETURN_TYPE)
        list(APPEND PONG_WARNING_FLAGS -Werror=return-type)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND PONG_DEBUG_FLAGS /RTC1)
    list(APPEND PONG_WARNING_FLAGS /W4 /wd4100 /permissive-)
endif()
