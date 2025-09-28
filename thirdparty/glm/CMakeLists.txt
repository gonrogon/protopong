add_library(glm INTERFACE)

target_include_directories(glm
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(glm
    INTERFACE
        GLM_FORCE_CXX20
        GLM_FORCE_RADIANS
        GLM_ENABLE_EXPERIMENTAL
)
