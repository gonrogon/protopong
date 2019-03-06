# Directories.
set(GLM_SOURCE  "${CMAKE_CURRENT_BINARY_DIR}/glm")
set(GLM_INSTALL "${main_LIBRARY_PREFIX}")
# Add the dependency.
add_external_dep(glm)
# Add the external project.
ExternalProject_Add(glm
    DOWNLOAD_DIR ${DOWNLOAD_DIR}
    SOURCE_DIR   ${GLM_SOURCE}
    INSTALL_DIR  ${GLM_INSTALL}
    URL          ${GLM_URL}
    URL_MD5      ${GLM_MD5}
    CMAKE_ARGS   -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
