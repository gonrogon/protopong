set(FLAGS_CXX_RELWITHDEBINFO "")
set(FLAGS_CXX_DEBUG          "")
set(FLAGS_CXX_RELEASE        "")
# Set up the flags for GCC.
if(CMAKE_COMPILER_IS_GNUCXX)
    # Include the utilities to check compiler flags. 
    include(CheckCXXCompilerFlag)
    # Additional warnings for GCC
    set(FLAGS_WARN -Wnon-virtual-dtor -Wno-long-long -Wcast-align -Wchar-subscripts -Wall -Wpointer-arith -Wformat-security -Woverloaded-virtual -Wextra -Wno-unused-parameter -Wconversion -fno-check-new -fno-common -pedantic)
    # This flag is useful as not returning from a non-void function is an error with MSVC, but it is not supported on
    # all GCC compiler versions.
    check_cxx_compiler_flag("-Werror=return-type" HACE_GCC_ERROR_RETURN_TYPE)
    # Set the flag.
    if (HAVE_GCC_ERROR_RETURN_TYPE)
        set(FLAGS_ERROR -Werror=return-type)
    endif()
    # Set up the debug flags for extra warnings.
    set(FLAGS_CXX_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${FLAGS_WARN} ${FLAGS_ERROR})
    set(FLAGS_CXX_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG}          ${FLAGS_WARN} ${FLAGS_ERROR})
    set(FLAGS_CXX_RELEASE        -O2 -DNDEBUG)
endif()
