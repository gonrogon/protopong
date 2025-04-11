set(glad_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(glad
    ${glad_SOURCES_DIR}/src/glad.c
)

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")