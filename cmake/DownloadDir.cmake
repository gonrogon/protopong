# Set up the downloads location if none was specified.
if (NOT DEFINED DOWNLOAD_DIR)
    set(DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/download")
endif()
# Create the download directory.
file(MAKE_DIRECTORY "${DOWNLOAD_DIR}")

