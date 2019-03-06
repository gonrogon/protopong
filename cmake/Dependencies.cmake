set(PROTOPONG_DEPS_IMPORT_MODE "directory" CACHE STRING "Mode to get the project dependencies")
set(PROTOPONG_DEPS_IMPORT_DIR "" CACHE PATH "Location of the dependencies import directory")
set(PROTOPONG_DEPS_BUILD_MODE "Release" CACHE STRING "Mode to build the project dependencies")
set(PROTOPONG_DEPS_REPOSITORY "https://github.com/gonrogon/protopong-deps.git" CACHE STRING "Repository with the dependencies")
# Set the allowed values for the variable to be shown in the cmake-gui.
set_property(CACHE PROTOPONG_DEPS_IMPORT_MODE PROPERTY STRINGS "directory" "build")
set_property(CACHE PROTOPONG_DEPS_BUILD_MODE  PROPERTY STRINGS "Release" "Debug")
# ---
# Dependencies loaded from a directory.
# ---
if (PROTOPONG_DEPS_IMPORT_MODE STREQUAL "directory")
    if (IS_DIRECTORY "${PROTOPONG_DEPENDENCIES_DIR}")
        message(STATUS "o---------------------------o")
        message(STATUS "| Load project dependencies |")
        message(STATUS "o---------------------------o")
        message(STATUS "Loading dependencies from: ${PROTOPONG_DEPS_IMPORT_DIR}")
        
        set(PROTOPONG_PREFIXES "${PROTOPONG_PREFIXES}" "${PROTOPONG_DEPS_IMPORT_DIR}")
    else()
        message(FATAL_ERROR "A valid path to the directory from where to import dependencies must be specified or leave the project build the dependencies automatically.")
    endif()
endif()
# ---
# Dependencies built from source.
# ---
if (PROTOPONG_DEPS_IMPORT_MODE STREQUAL "build")
    message(STATUS "o----------------------------o")
    message(STATUS "| Build project dependencies |")
    message(STATUS "o----------------------------o")
    
    set(DEPS_SOURCE  "${CMAKE_CURRENT_BINARY_DIR}/deps")
    set(DEPS_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/deps-install")
    
    set(PROTOPONG_PREFIXES "${PROTOPONG_PREFIXES}" "${DEPS_INSTALL}")
    # Download the project.
    FetchContent_Declare(protopong-deps
        DOWNLOAD_DIR   ${DOWNLOAD_DIR}
        SOURCE_DIR     ${DEPS_SOURCE}
        GIT_REPOSITORY ${PROTOPONG_DEPS_REPOSITORY}
    )
    FetchContent_GetProperties(protopong-deps)
    if (NOT protopong-deps_POPULATED)
        FetchContent_Populate(protopong-deps)
    endif()
    # Project directories and config options.
    set(DEPS_SOURCE "${protopong-deps_SOURCE_DIR}")
    set(DEPS_BINARY "${protopong-deps_BINARY_DIR}")
    set(DEPS_CONFIG_OPTIONS "")
    # Populate the config with the options set in the project with the dependencies.
    include("${DEPS_SOURCE}/cmake/OptionsList.cmake")        
    foreach(M_OPTION ${USE_OPTION_LIST})
        if (DEFINED ${M_OPTION})
            list(APPEND DEPS_CONFIG_OPTIONS "-D${M_OPTION}=${${M_OPTION}}")
        endif()
    endforeach()
    message(STATUS "Dependencies source: ${DEPS_SOURCE}")
    message(STATUS "Dependencies binary: ${DEPS_BINARY}")
    message(STATUS "Dependencies config: ${DEPS_CONFIG_OPTIONS}")
    # Configure.
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL}
        -DCMAKE_BUILD_TYPE=${PROTOPONG_DEPS_BUILD_MODE}
        ${DEPS_CONFIG_OPTIONS}
        ${DEPS_SOURCE}
        WORKING_DIRECTORY ${DEPS_BINARY}
    )
    # Build.
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${DEPS_BINARY})   
endif()

message(STATUS "o--------------------------o")
message(STATUS "| End project dependencies |")
message(STATUS "o--------------------------o")
