/////////////////////////////////////////////////////////////
/// Proto Pong
///
/// Copyright (c) 2015 - 2025 Gonzalo González Romero (gonrogon)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
////////////////////////////////////////////////////////////

#pragma once

#include "Event.hpp"
#include <memory>
#include <queue>

/* Forward declare SDL types to not include SDL.h in the header */
struct SDL_Window;

namespace pong {

class Renderer;
class Audio;
class Game;

/** @brief Defines a custom deleter for SDL window. */
struct SDLDeleter { void operator()(SDL_Window* win) const; };
/** @brief Defines a custom deleter for SDL GL context. */
struct SDLGLDeleter { void operator()(void* ctx) const;  };

/**
 * @brief The main application class, responsible for initialization, the main loop, and shutdown.
 *
 * @details This class orchestrates all major subsystems (Renderer, Audio, Game). It is designed using the RAII
 * principle and must be created via the static `create()` factory function, which handles all necessary initialization
 * that can fail.
 * The main loop is a fixed-timestep implementation for deterministic physics updates, with variable rendering for
 * smoothness.
 */
class App
{
public:

    /**
     * @brief Factory method to create and initialize the application.
     *
     * This is the sole entry point for creating an `App` instance. It handles all platform and subsystem
     * initialization, returning a null pointer if any critical setup step fails.
     * @param argc The command-line argument count from `main()`.
     * @param argv The command-line argument values from `main()`.
     * @return A unique pointer on success, null on failure.
     */
    [[nodiscard]] static std::unique_ptr<App> create(int argc, char** argv);

    App(const App&) = delete;

    App(App&&) = delete;

    App& operator=(const App&) = delete;

    App& operator=(App&&) = delete;

    /**
     * @brief Destructor.
     *
     * Automatically and safely cleans up all subsystems and SDL resources in the correct order.
     */
    ~App();

private:

    App();

    /**
     * @brief Internal initialization method called by the factory.
     * @param argc The command-line argument count.
     * @param argv The command-line argument values.
     * @return True on success, false otherwise.
     */
    bool init(int argc, char** argv);

public:

    /**
     * @brief Executes the main application loop.
     *
     * This method will block until the user quits or the game logic signals that it is done. It contains the
     * fixed-timestep update logic and rendering calls.
     */
    void exec();

private:

    /**
     * @brief Polls SDL for system events and translates them into game-specific `Event` objects.
     *
     * This method drains the SDL event queue on each frame and populates the internal `mEvents` queue for the `Game`
     * class to process.
     */
    void handleEvents();

    /**
     * @brief Creates the SDL window and the associated OpenGL context.
     *
     * @param flags Flags.
     * @param major OpenGL context major version.
     * @param minor OpenGL context minor version.
     * @return True on success, false otherwise.
     */
    bool openWindow(unsigned int flags, int major, int minor);

    /**
     * @brief Tries to enable V-sync.
     * @return True on success, false otherwise.
     */
    bool enableVSync();

private:

    /** @brief SDL window. */
    std::unique_ptr<SDL_Window, SDLDeleter> mWin;

    /** @brief SDL context. */
    std::unique_ptr<void, SDLGLDeleter> mCtx;

    /** @brief Rendering subsystem. */
    std::unique_ptr<Renderer> mRenderer;

    /** @brief Audio subsystem. */
    std::unique_ptr<Audio> mAudio;

    /** @brief Main game logic controller. */
    std::unique_ptr<Game> mGame;

    /** @brief A queue for game-specific events. */
    std::queue<Event> mEvents;
};

} // namespace pong
