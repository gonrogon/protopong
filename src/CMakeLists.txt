# Dependencies.
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
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
)
# Compiler options.
target_compile_options(protopong
    PRIVATE
		$<$<CONFIG:Debug>:${PONG_CXX_FLAGS_DEBUG}>
		$<$<CONFIG:Release>:${PONG_CXX_FLAGS_RELEASE}>
		$<$<CONFIG:RelWithDebInfo>:${PONG_CXX_FLAGS_RELWITHDEBINFO}>
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
# Properties.
set_target_properties(protopong
    PROPERTIES
        INSTALL_RPATH "../lib;lib"
        INSTALL_RPATH_USE_LINK_PATH FALSE
)
# Install.
install(TARGETS protopong
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# Install all shared libraries built as part of the project.
if (PROTOPONG_PREFIXES AND NOT PONG_INSTALL_PACKAGE)
	file(GLOB SHARED_LIST LIST_DIRECTORIES false
		"${PROTOPONG_PREFIXES}/lib/*.so*"
		"${PROTOPONG_PREFIXES}/lib/*.dll*"
		"${PROTOPONG_PREFIXES}/bin/*.so*"
		"${PROTOPONG_PREFIXES}/bin/*.dll*"
	)

	if (WIN32)
		install(FILES ${SHARED_LIST} DESTINATION ${INSTALL_RUNTIME_DIR})
	else()
		install(FILES ${SHARED_LIST} DESTINATION ${INSTALL_LIBRARY_DIR})
	endif()
endif()
