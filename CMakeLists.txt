cmake_minimum_required(VERSION 3.30)
# Tell CMake where to find the project modules.
set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find"
    "${CMAKE_MODULE_PATH}"
)
include(Version)
# Get the project version.
get_version_from_file("${CMAKE_SOURCE_DIR}/VERSION" PONG)
# Define the project.
project(protopong VERSION ${PONG_VERSION} LANGUAGES C CXX)
message(STATUS "VERSION --> " ${PONG_VERSION})
# Includes.
include(BuildType)
include(BuildLocation)
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
# Third party.
add_subdirectory("thirdparty/glm")
add_subdirectory("thirdparty/glad")
add_subdirectory("thirdparty/utf8d")
# Targets.
add_subdirectory(src)