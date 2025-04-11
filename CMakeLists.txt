cmake_minimum_required(VERSION 4.0 FATAL_ERROR)
# Tell CMake where to find the project modules.
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find"
    "${CMAKE_MODULE_PATH}"
)
include(Version)
# Get the project version.
get_version_from_file("VERSION" PONG)
# Define the project.
project(protopong-game VERSION ${PONG_VERSION} LANGUAGES C CXX)
# Includes.
include(BuildType)
include(BuildLocation)
include(InstallLocation)
include(CompilerFlags)
include(DownloadDir)
include(Options)
# Included dependencies.
add_subdirectory("deps/glad")
add_subdirectory("deps/utf8d")
# Targets.
add_subdirectory(src)