macro (get_version_from_file file prefix)
    # Set the default version.
    set("${prefix}_VERSION_MAJOR" 0)
    set("${prefix}_VERSION_MINOR" 0)
    set("${prefix}_VERSION_PATCH" 0)
    # Try to get the version from the file.
    if (EXISTS ${file})
        # Read the file content.
        file (STRINGS ${file} M_VERSION_STRING LIMIT_COUNT 1)
        # Convert the file content to a list.
        string(STRIP ${M_VERSION_STRING} M_VERSION_STRING)
        string(REPLACE "." ";" M_VERSION_LIST ${M_VERSION_STRING})
        # Get the length of the list to iterate.
        list(LENGTH M_VERSION_LIST M_VERSION_LIST_SIZE)
        # Get the version components.
        if (M_VERSION_LIST_SIZE GREATER 0)
            math(EXPR  M_VERSION_LIST_SIZE "${M_VERSION_LIST_SIZE} - 1")
            # Set the version components.
            foreach(M_INDEX RANGE ${M_VERSION_LIST_SIZE})
                if (M_INDEX EQUAL 0)
                    list(GET M_VERSION_LIST ${M_INDEX} "${prefix}_VERSION_MAJOR")
                endif()
                if (M_INDEX EQUAL 1)
                    list(GET M_VERSION_LIST ${M_INDEX} "${prefix}_VERSION_MINOR")
                endif()
                if (M_INDEX EQUAL 2)
                    list(GET M_VERSION_LIST ${M_INDEX} "${prefix}_VERSION_PATCH")
                endif()
            endforeach()
        endif()
    endif()
    # Set the complete version.
    set(${prefix}_VERSION "${${prefix}_VERSION_MAJOR}.${${prefix}_VERSION_MINOR}.${${prefix}_VERSION_PATCH}")
endmacro()