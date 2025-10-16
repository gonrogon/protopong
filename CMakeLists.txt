cmake_minimum_required(VERSION 3.30)
# Tell CMake where to find the project modules
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find"
    "${CMAKE_MODULE_PATH}"
)
include(Version)
# Get the project version
get_version_from_file("${CMAKE_SOURCE_DIR}/VERSION" PONG)
# Define the project
project(protopong VERSION ${PONG_VERSION} LANGUAGES C CXX)
message(STATUS "PROTOPONG: Version " ${PONG_VERSION})
# Includes
include(InstallLocation)
include(CompilerFlags)
include(Options)
include(Platform)
include(FetchContent)
# Set project variables
set(PONG_PROJECT_ROOT  ${CMAKE_CURRENT_SOURCE_DIR})
set(PONG_PROJECT_SRC   ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PONG_PROJECT_DEPS  ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(PONG_CMAKE_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Dependencies
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.2
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.32.10
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(glm SDL2)
# Dependencies subdirectories
add_subdirectory("thirdparty/glad")
add_subdirectory("thirdparty/utf8d")
# Targets.
add_subdirectory(src)