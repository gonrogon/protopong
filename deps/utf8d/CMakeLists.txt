set(utf8d_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(utf8d
    ${utf8d_SOURCES_DIR}/src/utf8d.cpp
)

target_include_directories(utf8d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")