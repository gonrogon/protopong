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
/// @file   src/App.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "App.hpp"
#include "Game.hpp"
#include "Event.hpp"
#include "Audio.hpp"
#include "Project.hpp"
#include "RealTimeClock.hpp"
#include "RendererGL3.hpp"
#include "RendererGL2.hpp"
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <cstring>
#include <cassert>

////////////////////////////////////////////////////////////

#define PONG_NAME       "Proto Pong " PONG_VERSION
#define PONG_WPOS       SDL_WINDOWPOS_CENTERED
#define PONG_A_KEY_UP   SDLK_UP
#define PONG_A_KEY_DOWN SDLK_DOWN
#define PONG_B_KEY_UP   SDLK_w
#define PONG_B_KEY_DOWN SDLK_s

////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////

namespace pong {
namespace 	   {

////////////////////////////////////////////////////////////

/**
 * @brief Open an SDL window with an OpenGL context.
 *
 * @param flags Flags.
 * @param major OpenGL context major version.
 * @param minor OpenGL context minor version.
 * @param win   Pointer where to store the handler for the window.
 * @param ctx   Pointer where to store the handler for the context.
 *
 * @return True on success; otherwise, false.
 */
bool openOpenGLWindow(Uint32 flags, int major, int minor, void*& win, void*& ctx);

/**
 * @brief Destroy the SDL window and the OpenGL context.
 *
 * @param win Pointer where the handler for the window is stored.
 * @param ctx Pointer where the handler for the context is stored.
 */
void destroyWindow(void*& win, void*& ctx);

/**
 * @brief Enable vertical synchronization.
 *
 * @return True on success; otherwise, false.
 */
bool enableVSync();

////////////////////////////////////////////////////////////

} // namespace

////////////////////////////////////////////////////////////
// CLASS: App
////////////////////////////////////////////////////////////

App::App()
    :
    mAudio   (new Audio),
    mGame    (new Game(*mAudio))
{}

////////////////////////////////////////////////////////////

App::~App()
{
    assert(mCtx == nullptr && mWin == nullptr && "Call the \"quit\" member function before deleting the application");
}

////////////////////////////////////////////////////////////

bool App::init(int argc, char** argv)
{
    SDL_DisplayMode vmode;
    Uint32          flags;
    // Try to initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "Unable to initiate SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    // Set the OpenGL attributes, the version and the profile.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Set the flags for the window and enable full screen if it is not in debug mode.
    if (argc > 1 && std::strncmp(argv[1], "--debug", 7) == 0)
    {
        flags = SDL_WINDOW_OPENGL;
    }
    else
    {
        flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS;
    }
    // Try to create the window.
    if(openOpenGLWindow(flags, 3, 3, mWin, mCtx))
    {
        mRenderer.reset(new RendererGL3());
    }
    else
    {
        if(openOpenGLWindow(flags, 2, 1, mWin, mCtx))
        {
            mRenderer.reset(new RendererGL2());
        }
        else
        {
            std::cerr << "Unable to create SDL window: " << SDL_GetError() << std::endl;
            return false;
        }
    }
    // Get the final video mode.
    SDL_GetWindowDisplayMode(static_cast<SDL_Window*>(mWin), &vmode);
    // Hide the mouse cursor because it is not required.
    SDL_ShowCursor(SDL_DISABLE);
    // Try to initialize GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit()  != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        return false;
    }
    // Try to initialize the renderer.
    if (!mRenderer->init(static_cast<unsigned int>(vmode.w), static_cast<unsigned int>(vmode.h)))
    {
        std::cerr << "Unable to initialize the renderer" << std::endl;
        return false;
    }
    // Try to initialize the audio system. The audio system is not a critical component, so on failure the application can
    // run without it.
    if (!mAudio->init())
    {
        std::cerr << "Unable to initialize the audio system" << std::endl;
    }
    // Initiate random numbers.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // Wait to ensure the window is ready.
    SDL_Delay(500);
    // Everything is fine, :)
    return true;
}

////////////////////////////////////////////////////////////

void App::quit()
{
    mAudio   ->quit();
    mRenderer->quit();

    if (mCtx) { SDL_GL_DeleteContext(static_cast<SDL_GLContext>(mCtx)); mCtx = nullptr; }
    if (mWin) { SDL_DestroyWindow   (static_cast<SDL_Window*>  (mWin)); mWin = nullptr; }
}

////////////////////////////////////////////////////////////

void App::exec()
{
          bool done  = false;
    const bool vsync = enableVSync();

    const double tickTime  = 1.0 / 60.0f; // Time between updates.
    const double drawTime  = 1.0 / 60.0f; // Time between draw calls (only if there is no vertical synchronization).
          double tickAccum = 0.0;         // Time accumulator with the time elapsed between updates.
          double drawAccum = 0.0;         // Time accumulator with the time elapsed between draw calls.

    RealTimeClock RTC;

    while (!done)
    {
        handleEvents();
        // Get the time elapsed since the last iteration.
        double elapsed = RTC.restart();
        // Clamp the time elapsed to avoid the spiral of death.
        if (elapsed > tickTime * 4)
        {
            elapsed = tickTime * 4;
        }
        // Increase the timers with the elapsed time.
        tickAccum += elapsed;
        drawAccum += elapsed;
        // Update in fixed time steps.
        for (; tickAccum >= tickTime; tickAccum -= tickTime)
        {
            while (!mEvents.empty())
            {
                mGame->handle(mEvents.front());
                             (mEvents.pop  ());
            }
            // Update the game and check if it has finished.
                   mGame->update(static_cast<float>(tickTime));
            done = mGame->done();
        }
        // Draw.
        if (!done)
        {
            mRenderer->begin();
            mGame->draw(static_cast<float>(drawAccum), static_cast<float>(tickAccum / tickTime), *mRenderer);
            mRenderer->end();
            // Reset the time accumulator with the time elapsed between draw calls.
            drawAccum = 0;
            // Swap the buffers.
            SDL_GL_SwapWindow(static_cast<SDL_Window*>(mWin));
            // If there is no vertical synchronization, the application is delayed until the next update or draw.
            if (!vsync)
            {
                double delay = std::min(tickTime - tickAccum, drawTime - drawAccum) - RTC.elapsed() * 1000.0;

                if (delay > 0.0)
                {
                    SDL_Delay(static_cast<Uint32>(delay));
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////

void App::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                mEvents.emplace(Event::Type::Quit);
            }
            break;

            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    case SDL_WINDOWEVENT_MINIMIZED:
                    {
                        mEvents.emplace(Event::Type::Minimize);
                    }
                    break;

                    case SDL_WINDOWEVENT_RESTORED:
                    {
                        mEvents.emplace(Event::Type::Maximize);
                    }
                    break;

                    default:break;
                }
            }
            break;

            case SDL_KEYDOWN:
            {
                if (event.key.repeat != 0)
                {
                    continue;
                }

                switch (event.key.keysym.sym)
                {
                    // Common interface.
                    case SDLK_ESCAPE: { mEvents.emplace(Event::Type::Quit); } break;
                    case SDLK_SPACE:  { mEvents.emplace(Event::Type::Next); } break;
                    case SDLK_y:      { mEvents.emplace(Event::Type::Yes);  } break;
                    case SDLK_n:      { mEvents.emplace(Event::Type::No);   } break;
                    case SDLK_h:      { mEvents.emplace(Event::Type::Help); } break;
                    case SDLK_1:
                    case SDLK_KP_1:   { mEvents.emplace(Event::Type::One);  } break;
                    case SDLK_2:
                    case SDLK_KP_2:   { mEvents.emplace(Event::Type::Two);  } break;
                    // Player controls.
                    case PONG_A_KEY_UP:   { mEvents.emplace(Event::Type::PlayerAMoveUp);   } break;
                    case PONG_A_KEY_DOWN: { mEvents.emplace(Event::Type::PlayerAMoveDown); } break;
                    case PONG_B_KEY_UP:   { mEvents.emplace(Event::Type::PlayerBMoveUp);   } break;
                    case PONG_B_KEY_DOWN: { mEvents.emplace(Event::Type::PlayerBMoveDown); } break;

                    default:break;
                }
            }
            break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    // Player controls.
                    case PONG_A_KEY_UP:   { mEvents.emplace(Event::Type::PlayerAMoveUpReleased);   } break;
                    case PONG_A_KEY_DOWN: { mEvents.emplace(Event::Type::PlayerAMoveDownReleased); } break;
                    case PONG_B_KEY_UP:   { mEvents.emplace(Event::Type::PlayerBMoveUpReleased);   } break;
                    case PONG_B_KEY_DOWN: { mEvents.emplace(Event::Type::PlayerBMoveDownReleased); } break;

                    default:break;
                }
            }

            default:break;
        }
    }
}

////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////

bool openOpenGLWindow(const Uint32 flags, const int major, const int minor, void*& win, void*& ctx)
{
    Uint32 sdlFlags = flags | SDL_WINDOW_HIDDEN;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

    if (major >= 3)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
    }
    else
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    }
    // Try to create the window.
    win = SDL_CreateWindow(PONG_NAME, PONG_WPOS, PONG_WPOS, 1024, 768, sdlFlags);
    if (win == nullptr)
    {
        return false;
    }
    // Try to create the OpenGL context.
    ctx = SDL_GL_CreateContext(static_cast<SDL_Window*>(win));
    if (ctx == nullptr)
    {
        destroyWindow(win, ctx);

        return false;
    }

    int glMajor = 0;
    int glMinor = 0;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glMajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glMinor);

    if (glMajor != major && glMinor != minor)
    {
        destroyWindow(win, ctx);

        return false;
    }

    std::cout << "Window open, context version " << major << "." << minor << std::endl;

    SDL_ShowWindow(static_cast<SDL_Window*>(win));

    return true;
}

////////////////////////////////////////////////////////////

void destroyWindow(void*& win, void*& ctx)
{
    if (ctx != nullptr) { SDL_GL_DeleteContext(static_cast<SDL_GLContext>(ctx)); ctx = nullptr; }
    if (win != nullptr) { SDL_DestroyWindow   (static_cast<SDL_Window*>  (win)); win = nullptr; }
}

////////////////////////////////////////////////////////////

bool enableVSync()
{
    if (SDL_GL_SetSwapInterval(-1) != 0)
    {
        return SDL_GL_SetSwapInterval(1) == 0;
    }

    return true;
}

////////////////////////////////////////////////////////////

} // namespace
} // namespace pong
