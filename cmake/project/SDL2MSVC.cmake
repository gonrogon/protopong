# Directories.
set(SDL2MSVC_SOURCE  "${CMAKE_CURRENT_BINARY_DIR}/sdl2")
set(SDL2MSVC_INSTALL "${main_LIBRARY_PREFIX}")
# Add the dependency.
add_external_dep(sdl2)
# Get the path to the folder that contains this file.
get_filename_component(_SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
# Files.
set(SDL2MSVC_LISTS_IN  "${_SELF_DIR}/SDL2MSVC.lists.cmake")
set(SDL2MSVC_LISTS_OUT "${CMAKE_CURRENT_BINARY_DIR}/SDL2MSVC.lists.cmake")
# Copy our CMakeLists.txt to the binary directory.
configure_file(${SDL2MSVC_LISTS_IN} ${SDL2MSVC_LISTS_OUT} @ONLY)
# Add the external project.
ExternalProject_Add(sdl2
    DOWNLOAD_DIR  ${DOWNLOAD_DIR}
    SOURCE_DIR    ${SDL2MSVC_SOURCE}
    INSTALL_DIR   ${SDL2MSVC_INSTALL}
    URL           ${SDL2MSVC_URL}
    URL_MD5       ${SDL2MSVC_MD5}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2MSVC_LISTS_OUT} "<SOURCE_DIR>/CMakeLists.txt"
    CMAKE_ARGS    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
