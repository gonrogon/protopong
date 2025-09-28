# Set the configuration types if this is a multiconfig generator; otherwise, set the build type if none was specified.
if(CMAKE_CONFIGURATION_TYPES)
    # Set only the Debug and Release configurations.
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)
else()
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Debug' as none was specified.")
        # Set the build type to Debug.
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (Debug or Release)." FORCE)
        # Set the possible values of build type for the GUI.
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
    else()
        string(TOUPPER "${CMAKE_BUILD_TYPE}" M_CMAKE_BUILD_TYPE_UPPER)
        if (M_CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
            set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
        elseif (M_CMAKE_BUILD_TYPE_UPPER STREQUAL "RELEASE")
            set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
        else()
            message(WARNING "Unsupported CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}. Using Debug as default.")
            set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
        endif()
    endif()
endif()
