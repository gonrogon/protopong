/////////////////////////////////////////////////////////////
/// Proto Pong
///
/// Copyright (c) 2015 - 2016 Gonzalo González Romero
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
///
/// @file   src/App.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Event.hpp"
#include <memory>
#include <queue>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an application.
///
/// This class implements the main loop and creates the instances for each subsystem (render, audio and
/// game). It also manages the queue with the events.
////////////////////////////////////////////////////////////
class App
{
private:

    /** @brief Define a type for unique pointer to renderer. */
    typedef std::unique_ptr<Renderer> RendererPtr;

    /** @brief Define a type for unique pointers to audio systems. */
    typedef std::unique_ptr<Audio> AudioPtr;

    /** @brief Define a type for unique pointers to games. */
    typedef std::unique_ptr<Game> GamePtr;

    /** @brief Define a type for queues of events. */
    typedef std::queue<Event> Events;

public:

    /**
     * @brief Constructor.
     */
    App();

    /**
     * @brief Constructor (copy).
     */
    App(const App&) = delete;

    /**
     * @brief Destructor.
     */
    ~App();

    /**
     * @brief Assignment operator (copy).
     */
    App& operator=(const App&) = delete;

    /**
     * @brief Initialize the application.
     *
     * @param argc Number of arguments.
     * @param argv List of arguments.
     *
     * @return True on success; otherwise, false.
     */
    bool init(int argc, char** argv);

    /**
     * @brief Finalize the application.
     */
    void quit();

    /**
     * @brief Execute the main loop.
     */
    void exec();

private:

    /**
     * @brief Handle the system events (window, keyboard, etc).
     */
    void handleEvents();

private:

    /** @brief SDL window. */
    void* mWin = nullptr;

    /** @brief SDL context. */
    void* mCtx = nullptr;

    /** @brief Renderer. */
    RendererPtr mRenderer;

    /** @brief Audio system. */
    AudioPtr mAudio;

    /** @brief Game controller. */
    GamePtr mGame;

    /** @brief Events system. */
    Events mEvents;
};

////////////////////////////////////////////////////////////

} // namespace pong
