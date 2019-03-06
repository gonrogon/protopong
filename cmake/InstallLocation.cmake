# Include the default install directories for GNU.
include(GNUInstallDirs)
# Set up the runtime directory.
if(NOT INSTALL_RUNTIME_DIR)
    set(INSTALL_RUNTIME_DIR "${CMAKE_INSTALL_BINDIR}")
endif()
# Set up the directory for libraries.
if(NOT INSTALL_LIBRARY_DIR)
    set(INSTALL_LIBRARY_DIR "${CMAKE_INSTALL_LIBDIR}")
endif()
# Set up the directory for archives.
if(NOT INSTALL_ARCHIVE_DIR)
    set(INSTALL_ARCHIVE_DIR "${CMAKE_INSTALL_LIBDIR}")
endif()
# Set up the directory for headers.
if(NOT INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
endif()
# Set up the directory for data.
if(NOT INSTALL_DATA_DIR)
    set(INSTALL_DATA_DIR "${CMAKE_INSTALL_DATADIR}")
endif()
# Set up the documentation directory.
if(NOT INSTALL_DOC_DIR)
    set(INSTALL_DOC_DIR "${CMAKE_INSTALL_DOCDIR}")
endif()
