# Find the UTF8D library.
#
# Defines:
#    UTF8D_FOUND       - System has UTF8D.
#    UTF8D_INCLUDE_DIR - The UTF8D inclue directory.

find_path(UTF8D_INCLUDE_DIR "utf8d.hpp")
find_library(UTF8D_LIBRARY NAMES "utf8d")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(UTF8D DEFAULT_MSG UTF8D_INCLUDE_DIR UTF8D_LIBRARY)

mark_as_advanced(UTF8D_INCLUDE_DIR UTF8D_LIBRARY)
