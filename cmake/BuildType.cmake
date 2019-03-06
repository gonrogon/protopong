# Set the confguration types if this is a multiconfig generator; otherwise, set the build type if none was specified.
if(CMAKE_CONFIGURATION_TYPES)
    # Set only the Debug and Release confgurations.
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
else()
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Debug' as none was specified.")
        # Set the buld type to Debug.
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (Debug or Release)." FORCE)
        # Set the possible values of build type for the GUI.
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
    endif()
endif()
