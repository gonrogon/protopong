# Directories
set(UTF8D_SOURCE  "${CMAKE_CURRENT_BINARY_DIR}/utf8d")
set(UTF8D_INSTALL "${main_LIBRARY_PREFIX}")
# Add the dependency.
add_external_dep(utf8d)
# Add the external project.
ExternalProject_Add(utf8d
    DOWNLOAD_DIR ${DOWNLOAD_DIR}
    SOURCE_DIR   ${UTF8D_SOURCE}
    INSTALL_DIR  ${UTF8D_INSTALL}
    URL          ${UTF8D_URL}
    URL_MD5      ${UTF8D_MD5}
    CMAKE_ARGS   -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
