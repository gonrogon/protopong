# Find the SDL2 library.
#
# Defines:
#    SDL2_FOUND       - System has SDL2.
#    SDL2_LIBRARY     - The SDL2 library.
#    SDL2_INCLUDE_DIR - The SDL2 include directories.

set(SDL2_SEARCH_PATHS
   ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
)

find_path(SDL2_INCLUDE_DIR SDL.h
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES include/SDL2 include
    PATHS ${SDL2_SEARCH_PATHS}
)

find_library(SDL2_LIBRARY_TEMP
    NAMES SDL2
    HINTS
    $ENV{SDL2DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ${SDL2_SEARCH_PATHS}
)

if(NOT SDL2_BUILDING_LIBRARY)
    if(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
        # Non-OS X framework versions expect you to also dynamically link to SDL2main. This is mainly for Windows and
        # OS X. Other (Unix) platforms seem to provide SDL2main for compatibility even though they don't necessarily
        # need it.
        find_library(SDL2MAIN_LIBRARY
            NAMES SDL2main
            HINTS
            $ENV{SDL2DIR}
            PATH_SUFFIXES lib64 lib
            PATHS ${SDL2_SEARCH_PATHS}
        )
    endif()
endif()
# SDL2 may require threads on your system. The Apple build may not need an explicit flag because one of the frameworks
# may already provide it. But for non-OSX systems, I will use the CMake Threads package.
if(NOT APPLE)
    find_package(Threads)
endif()
# MinGW needs an additional library, mwindows. It's total link flags should look like -lmingw32 -lSDL2main -lSDL2
# -lmwindows (Actually on second look, I think it only needs one of the m* libraries.)
if(MINGW)
    set(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")
endif()

if(SDL2_LIBRARY_TEMP)
    # For SDL2main
    if(NOT SDL2_BUILDING_LIBRARY)
        if(SDL2MAIN_LIBRARY)
            set(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
        endif()
    endif()
    # For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa. CMake doesn't display the -framework Cocoa
    # string in the UI even though it actually is there if I modify a pre-used variable. I think it has something
    # to do with the CACHE STRING. So I use a temporary variable until the end so I can set the "real" variable
    # in one-shot.
    if(APPLE)
        set(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} "-framework Cocoa")
    endif()
    # For threads, as mentioned Apple doesn't need this. In fact, there seems to be a problem if I used the Threads
    # package and try using this line, so I'm just skipping it entirely for OS X.
    if(NOT APPLE)
        set(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
    endif()
    # For MinGW library
    if(MINGW)
        set(SDL2_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2_LIBRARY_TEMP})
    endif()
    # Set the final string here so the GUI reflects the final state.
    set(SDL2_LIBRARY ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
    # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
    set(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
