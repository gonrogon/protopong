cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
# Create a project for SDL2.
project(sdl2 C)
# Enable the language C to find pthread in linux.
enable_language(C)
# Determine the prefix for the binary files to copy.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL2_PREFIX "lib/x64")
else()
    set(SDL2_PREFIX "lib/x86")
endif()
# Get the path to the folde that contains this file.
set(SDL2_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${SDL2_PREFIX}")
# Find the binary.
find_file(SDL2_BINARY_FILE "SDL2.dll"     PATHS ${SDL2_FILES_DIR} NO_DEFAULT_PATH)
find_file(SDL2_LIB_FILE    "SDL2.lib"     PATHS ${SDL2_FILES_DIR} NO_DEFAULT_PATH)
find_file(SDL2_MAIN_FILE   "SDL2main.lib" PATHS ${SDL2_FILES_DIR} NO_DEFAULT_PATH)
# Install the headers.
install(DIRECTORY
    include/
    DESTINATION include/SDL2
)
# Install binaries.
install(FILES
    "${SDL2_BINARY_FILE}"
    DESTINATION bin
)
# Install librarires.
install(FILES
    ${SDL2_LIB_FILE}
    ${SDL2_MAIN_FILE}
    DESTINATION lib
)
