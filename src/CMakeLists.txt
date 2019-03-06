# Dependencies.
find_package(OpenGL REQUIRED)
find_package(glew   REQUIRED)
find_package(sdl2   REQUIRED)
find_package(glm    REQUIRED)
# Imported interfaces.
add_library(opengl INTERFACE IMPORTED)
set_property(TARGET opengl PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OPENGL_INCLUDE_DIR})
set_property(TARGET opengl PROPERTY INTERFACE_LINK_LIBRARIES      ${OPENGL_gl_LIBRARY})
add_library(sdl2 INTERFACE IMPORTED)
string(STRIP ${SDL2_LIBRARIES} M_SDL2_LIBRARIES) # Fixed in SDL version 2.0.5
set_property(TARGET sdl2 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIRS})
set_property(TARGET sdl2 PROPERTY INTERFACE_LINK_LIBRARIES      ${M_SDL2_LIBRARIES})
# Configuration.
configure_file("Project.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/Project.hpp")
# Sources.
set(SOURCES
    "App.cpp"
    "App.hpp"
    "Audio.cpp"
    "Audio.hpp"
    "Ball.cpp"
    "Ball.hpp"
    "Main.cpp"
    "Project.hpp"
    "Controller.hpp"
    "ControllerAI.cpp"
    "ControllerAI.hpp"
    "ControllerHuman.cpp"
    "ControllerHuman.hpp"
    "Entity.hpp"
    "Event.cpp"
    "Event.hpp"
    "Game.cpp"
    "Game.hpp"
    "Label.cpp"
    "Label.hpp"
    "Paddle.cpp"
    "Paddle.hpp"
    "RealTimeClock.cpp"
    "RealTimeClock.hpp"
    "Renderer.hpp"
    "RendererGL2.cpp"
    "RendererGL2.hpp"
    "RendererGL3.cpp"
    "RendererGL3.hpp"
    "Require.hpp"
    "Scene.cpp"
    "Scene.hpp"
    "Table.cpp"
    "Table.hpp"
    "data/Char.cpp"
    "data/Char.hpp"
    "data/Sound.cpp"
    "data/Sound.hpp"
    "lib/utf8d.cpp"
    "lib/utf8d.hpp"
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
# Properties.
set_target_properties(protopong
    PROPERTIES
        INSTALL_RPATH "../lib;lib"
        INSTALL_RPATH_USE_LINK_PATH FALSE
)
# Libraries.
target_link_libraries(protopong
    PUBLIC
		GLEW::GLEW
        opengl
		sdl2
        glm
)
# Install.
install(TARGETS protopong
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
)
# Install all shared libraries built as part of the project.
if (PONG_LIBRARY_DIRECTORY AND NOT PONG_INSTALL_PACKAGE)
	file(GLOB SHARED_LIST LIST_DIRECTORIES FALSE
		"${PONG_LIBRARY_DIRECTORY}/lib/*.so*"
		"${PONG_LIBRARY_DIRECTORY}/lib/*.dll*"
		"${PONG_LIBRARY_DIRECTORY}/bin/*.so*"
		"${PONG_LIBRARY_DIRECTORY}/bin/*.dll*"
	)

	if (WIN32)
		install(FILES ${SHARED_LIST} DESTINATION ${INSTALL_RUNTIME_DIR})
	else()
		install(FILES ${SHARED_LIST} DESTINATION ${INSTALL_LIBRARY_DIR})
	endif()
endif()
# Install the game data.
if (PONG_INSTALL_DATA)
    if (NOT PONG_INSTALL_PACKAGE AND PONG_CREDITS_FILE)
            install(FILES ${PONG_CREDITS_FILE} DESTINATION .)
    endif()
endif()
