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

#include "App.hpp"
#include "Game.hpp"
#include "Event.hpp"
#include "Audio.hpp"
#include "Project.hpp"
#include "RealTimeClock.hpp"
#include "RendererGL3.hpp"
#include <glad/gl.h>
#include <SDL.h>
#include <iostream>
#include <cstring>
#include <cassert>
#include <thread>

namespace pong {

constexpr std::string_view PongWindowName = "Proto Pong " PONG_VERSION;
constexpr unsigned int PongWindowPosition = SDL_WINDOWPOS_CENTERED;
constexpr SDL_KeyCode PongAKeyUp   = SDLK_UP;
constexpr SDL_KeyCode PongAKeyDown = SDLK_DOWN;
constexpr SDL_KeyCode PongBKeyUp   = SDLK_w;
constexpr SDL_KeyCode PongBKeyDown = SDLK_s;

void SDLDeleter::operator()(SDL_Window* win) const
{
    if (win)
    {
        SDL_DestroyWindow(win);
    }
}

void SDLGLDeleter::operator()(void* ctx) const
{
    if (ctx)
    {
        SDL_GL_DeleteContext(ctx);
    }
}

std::unique_ptr<App> App::create(const int argc, char** argv)
{
    auto app = std::unique_ptr<App>(new App{});
    if (app->init(argc, argv))
    {
        return app;
    }

    return nullptr;
}

App::App() = default;

////////////////////////////////////////////////////////////

App::~App()
{
    mGame     = {};
    mAudio    = {};
    mRenderer = {};
    mCtx      = {};
    mWin      = {};

    SDL_Quit();
}

////////////////////////////////////////////////////////////

bool App::init(const int argc, char** argv)
{
    SDL_DisplayMode vMode;
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
        flags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS;
    }
    // Try to create the window.
    if(!openWindow(flags, 3, 3))
    {
        std::cerr << "Unable to create SDL window: " << SDL_GetError() << std::endl;
        return false;
    }
    // Get the final video mode.
    SDL_GetWindowDisplayMode(mWin.get(), &vMode);
    // Hide the mouse cursor because it is not required.
    SDL_ShowCursor(SDL_DISABLE);
    // Try to initialize GLAD.
    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress))) {
        std::cerr << "Unable to initialize GLAD" << std::endl;
        return false;
    }
    // Try to initialize the renderer.
    mRenderer = RendererGL3::create(vMode.w, vMode.h);
    if (!mRenderer)
    {
        std::cerr << "Unable to initialize the renderer" << std::endl;
        return false;
    }
    // Try to initialize the audio system. The audio system is not a critical component, so on failure the application can
    // run without it.
    mAudio = Audio::create();
    if (!mAudio)
    {
        std::cerr << "Unable to initialize the audio system" << std::endl;
    }
    // Initiate random numbers.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // Initiate the game.
    mGame = std::make_unique<Game>(*mAudio);
    // Wait to ensure the window is ready.
    SDL_Delay(500);
    // Everything is fine, :)
    return true;
}

////////////////////////////////////////////////////////////

void App::exec()
{
          bool done  = false;
    const bool vsync = enableVSync();

    const TimeDuration tickTime{1.0 / 60.0};
    const TimeDuration drawTime{1.0 / 60.0};
    TimeDuration tickAccum{};
    TimeDuration drawAccum{};

    RealTimeClock RTC;

    while (!done)
    {
        handleEvents();
        // Get the time elapsed since the last iteration.
        TimeDuration elapsed = RTC.restart();
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
                   mGame->update(TimeDuration(tickTime));
            done = mGame->done();
        }
        // Draw.
        if (!done)
        {
            mRenderer->beginFrame();
            mGame->draw(*mRenderer, static_cast<float>(tickAccum / tickTime));
            mRenderer->endFrame();
            // Reset the time accumulator with the time elapsed between draw calls.
            drawAccum = {};
            // Swap the buffers.
            SDL_GL_SwapWindow(mWin.get());
            // When VSync is disabled, the main loop can run at thousands of frames per second, consuming 100% of a CPU
            // core. This block acts as a fallback manual frame limiter to conserve system resources.
            if (!vsync)
            {
                /*
                 * The logic calculates the "spare time" available before the next scheduled game event and puts the
                 * thread to sleep for that duration.
                 *
                 * 1. Calculate Time to Next Event: It finds the time remaining until the next physics tick is due
                 *    (`tickTime - tickAccum`) and the time until the next draw call is due (`drawTime - drawAccum`).
                 * 2. Find the Soonest Event: `std::min` selects the smaller of these two durations. We must sleep for
                 *    no longer than this to avoid delaying the next event.
                 * 3. Self-Correction: It then subtracts `RTC.elapsed()`, which is the time already spent performing
                 *    work (like drawing) within the current frame. This ensures the delay calculation is accurate, and
                 *    we don't oversleep.
                 * 4. Sleep: If the resulting `delay` is a positive duration, it means we have spare time. The thread
                 *    is put to sleep using `SDL_Delay`, yielding the CPU to the operating system. The duration is
                 *    safely cast to milliseconds as required by the SDL API. If the delay is zero or negative, it means
                 *    we are already running late (a "frame drop"), and no delay is performed.
                 */
                const TimeDuration delay = std::min(tickTime - tickAccum, drawTime - drawAccum) - RTC.elapsed();
                if (delay > TimeDuration::zero())
                {
                    std::this_thread::sleep_for(delay);
                }
            }
        }
    }
}

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
                    case PongAKeyUp:   { mEvents.emplace(Event::Type::PlayerAMoveUp);   } break;
                    case PongAKeyDown: { mEvents.emplace(Event::Type::PlayerAMoveDown); } break;
                    case PongBKeyUp:   { mEvents.emplace(Event::Type::PlayerBMoveUp);   } break;
                    case PongBKeyDown: { mEvents.emplace(Event::Type::PlayerBMoveDown); } break;

                    default:break;
                }
            }
            break;

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    // Player controls.
                    case PongAKeyUp:   { mEvents.emplace(Event::Type::PlayerAMoveUpReleased);   } break;
                    case PongAKeyDown: { mEvents.emplace(Event::Type::PlayerAMoveDownReleased); } break;
                    case PongBKeyUp:   { mEvents.emplace(Event::Type::PlayerBMoveUpReleased);   } break;
                    case PongBKeyDown: { mEvents.emplace(Event::Type::PlayerBMoveDownReleased); } break;

                    default:break;
                }
            }

            default:break;
        }
    }
}

bool App::openWindow(const unsigned int flags, const int major, const int minor)
{
    const Uint32 sdlFlags = flags | SDL_WINDOW_HIDDEN;

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
    mWin.reset(SDL_CreateWindow(PongWindowName.data(), PongWindowPosition, PongWindowPosition, 1024, 768, sdlFlags));
    if (!mWin)
    {
        return false;
    }
    // Try to create the OpenGL context.
    mCtx.reset(SDL_GL_CreateContext(mWin.get()));
    if (!mCtx)
    {
        return false;
    }

    int glMajor = 0;
    int glMinor = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glMajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glMinor);

    if (glMajor != major && glMinor != minor)
    {
        return false;
    }

    std::cout << "Window open, context version " << major << "." << minor << std::endl;
    SDL_ShowWindow(mWin.get());

    return true;
}

bool App::enableVSync()
{
    if (SDL_GL_SetSwapInterval(-1) != 0)
    {
        return SDL_GL_SetSwapInterval(1) == 0;
    }

    return true;
}

} // namespace pong
