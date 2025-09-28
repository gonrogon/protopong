set(M_INC ${CMAKE_CURRENT_SOURCE_DIR}/utf8d)
set(M_SRC ${CMAKE_CURRENT_SOURCE_DIR}/utf8d)

add_library(utf8d STATIC "${M_SRC}/utf8d.cpp")

target_include_directories(utf8d
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
