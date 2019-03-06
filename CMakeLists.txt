cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Tell CMake where to find the project modules.
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find"
    "${CMAKE_MODULE_PATH}"
)

# Includes.
include(BuildType)
include(BuildLocation)
include(InstallLocation)
#include(Dependencies)
include(CompilerFlags)
include(Options)
include(Version)
include(DownloadDir)
include(ExternalProject)
include(FetchContent)
include(Dependencies)
# Get the project version.
get_version_from_file("${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PONG)
# Define the project.
project(protopong-game VERSION ${PONG_VERSION} LANGUAGES CXX)
# Prepends the path to the project dependencies to the CMake seach path.
set(CMAKE_PREFIX_PATH ${PROTOPONG_PREFIXES} ${CMAKE_PREFIX_PATH})
# Targets.
add_subdirectory(src)

