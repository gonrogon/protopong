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
		SDL2::SDL2
		$<$<BOOL:WIN32>:SDL2main>
		glad
		glm
		utf8d
)
# Install.
install(TARGETS protopong
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
