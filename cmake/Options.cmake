# Flag indicating whether to set or not an installation tree compatible with a UNIX package (DEB/RPM).
set(PONG_INSTALL_PACKAGE FALSE
    CACHE BOOL "Should we configure the installation for UNIX package?")
# Flag indicating whether to install or not the game data.
set(PONG_INSTALL_DATA TRUE
    CACHE BOOL "Should we install the game data?")
# Path to the directory with the game data.
set(PONG_DATA_DIRECTORY ${CMAKE_SOURCE_DIR}/data
    CACHE PATH "Directory with the game data")
