set(GLEW_SOURCE  "${CMAKE_CURRENT_BINARY_DIR}/GLEW")
set(GLEW_INSTALL "${main_LIBRARY_PREFIX}")
# Add the dependency.
add_external_dep(glew)
# Add an external project to download the library.
ExternalProject_Add(glew_download
    DOWNLOAD_DIR      ${DOWNLOAD_DIR}
    SOURCE_DIR        ${GLEW_SOURCE}
    URL               ${GLEW_URL}
    URL_MD5           ${GLEW_MD5}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
)
# Add an external project to build the library.
ExternalProject_Add(glew
    SOURCE_DIR        "${GLEW_SOURCE}/build/cmake"
    INSTALL_DIR       "${GLEW_INSTALL}"
    DOWNLOAD_COMMAND  ""
    UPDATE_COMMAND    ""
    CMAKE_ARGS        -DBUILD_UTILS:BOOL=FALSE
                      -DBUILD_SHARED_LIBS:BOOL=FALSE
                      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    DEPENDS            glew_download
)
