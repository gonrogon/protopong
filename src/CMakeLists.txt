# Dependencies.
find_package(OpenGL REQUIRED)
# Configuration.
configure_file("Project.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/Project.hpp")
# Sources.
set(PONG_SOURCES
    "App.cpp"
    "Audio.cpp"
    "Ball.cpp"
    "Main.cpp"
    "ControllerAI.cpp"
    "ControllerHuman.cpp"
    "Event.cpp"
    "Game.cpp"
    "Label.cpp"
    "Paddle.cpp"
    "RealTimeClock.cpp"
    "RendererGL3.cpp"
    "Scene.cpp"
    "Table.cpp"
    "data/Char.cpp"
)
# Add the executable.
add_executable(protopong WIN32)
# Sources
target_sources(protopong PRIVATE ${PONG_SOURCES})
# Properties
target_compile_features(protopong PUBLIC cxx_std_20)
# Includes.
target_include_directories(protopong
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
# Compiler definitions.
target_compile_definitions(protopong
    PRIVATE
        $<$<CONFIG:Debug>:PONG_ASSERTIONS_ENABLED>
        $<$<CONFIG:Debug>:PONG_DEBUG>
		GLM_FORCE_CXX20
		GLM_FORCE_RADIANS
		GLM_ENABLE_EXPERIMENTAL
)
# Compiler options.
target_compile_options(protopong
    PRIVATE
		${PONG_WARNING_FLAGS}
		$<$<CONFIG:Debug>:${PONG_DEBUG_FLAGS}>
)
# Linker options.
target_link_options(protopong
	PRIVATE
		$<$<CONFIG:Debug>:${PONG_DEBUG_LINK_FLAGS}>
)
# Libraries.
target_link_libraries(protopong
	PRIVATE      
		OpenGL::GL
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
		glad
		glm::glm-header-only
		utf8d
)
# Install.
install(TARGETS protopong
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
)
# Install dependencies.
if (WIN32)
    set(SDL2_REAL_TARGET "SDL2::SDL2")
    if (TARGET SDL2::SDL2)
        # If SDL2::SDL2 is an alias, ALIASET_TARGET will be set with name of the real target, otherwise it will be left empty.
        get_target_property(ALIASED_TARGET SDL2::SDL2 ALIASED_TARGET)
        if (ALIASED_TARGET)
            set(SDL2_REAL_TARGET ${ALIASED_TARGET})
        endif()
    endif()

    install(IMPORTED_RUNTIME_ARTIFACTS
        ${SDL2_REAL_TARGET}
	    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
    )

    add_custom_command(
        TARGET protopong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${SDL2_REAL_TARGET}>"
            "$<TARGET_FILE_DIR:protopong>"
        VERBATIM
    )
endif()


