# Dependencies.
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
# Configuration.
configure_file("Project.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/Project.hpp")
# Sources.
set(SOURCES
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
    "RendererGL2.cpp"
    "RendererGL3.cpp"
    "Scene.cpp"
    "Table.cpp"
    "data/Char.cpp"
    "data/Sound.cpp"
	"data/Shader.cpp"
)
# Add the executable.
add_executable(protopong WIN32 ${SOURCES})
# Includes.
target_include_directories(protopong
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
# Compiler features.
target_compile_features(protopong
    PRIVATE
        cxx_std_11
)
# Compiler definitions.
target_compile_definitions(protopong
    PRIVATE
        $<$<CONFIG:Debug>:PROTOPONG_ASSERTIONS_ENABLED>
        $<$<CONFIG:Debug>:PROTOPONG_DEBUG>
        GLM_FORCE_RADIANS
        GLM_ENABLE_EXPERIMENTAL
)
# Compiler options.
target_compile_options(protopong
    PRIVATE
        $<$<CONFIG:Debug>:${FLAGS_CXX_DEBUG}>
        $<$<CONFIG:Release>:${FLAGS_CXX_RELEASE}>
        $<$<CONFIG:RelWithDebInfo>:${FLAGS_CXX_RELWITHDEBINFO}>
)
# Libraries.
target_link_libraries(protopong
	PRIVATE      
		OpenGL::GL
        glad
		SDL2::SDL2
		$<$<BOOL:WIN32>:SDL2main>
		glm::glm
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
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
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
# Install the game data.
if (PONG_INSTALL_DATA)
    if (NOT PONG_INSTALL_PACKAGE)
            install(FILES "${CMAKE_SOURCE_DIR}/CREDITS.txt" DESTINATION .)
    endif()
endif()