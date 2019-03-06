# Directories.
set(SDL2_SOURCE  "${CMAKE_CURRENT_BINARY_DIR}/sdl2")
set(SDL2_INSTALL "${main_LIBRARY_PREFIX}")
# Add the dependency.
add_external_dep(sdl2)
# Add the external project.
ExternalProject_Add(sdl2
    DOWNLOAD_DIR  ${DOWNLOAD_DIR}
    SOURCE_DIR    ${SDL2_SOURCE}
    INSTALL_DIR   ${SDL2_INSTALL}
    URL           ${SDL2_URL}
    URL_MD5       ${SDL2_MD5}
    CMAKE_ARGS    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
				  -DSDL_SHARED:BOOL=TRUE
				  -DSDL_STATIC:BOOL=FALSE
				  -DDIRECTX:BOOL=FALSE
)
