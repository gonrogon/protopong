set(M_INC ${CMAKE_CURRENT_SOURCE_DIR}/glad)
set(M_SRC ${CMAKE_CURRENT_SOURCE_DIR}/glad)

add_library(glad STATIC "${M_SRC}/gl.c")

target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
