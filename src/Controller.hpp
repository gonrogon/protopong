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

#include "RealTimeClock.hpp"

namespace pong {

class Paddle;
class Table;
class Ball;
class Event;

/**
 * @brief Defines an interface for the behavior of a paddle.
 *
 * This class represents the "Strategy" pattern for a Paddle. Concrete implementations of this interface can define
 * different behaviors, such as control by a human player (reacting to keyboard events) or control by an AI (reacting
 * to the ball's position).
 */
class Controller
{
public:

    /**
     * @brief A type alias for a time duration, representing seconds as a floating-point value.
     */
    using Seconds = RealTimeClock::Duration;

    Controller() = default;

    Controller(const Controller&) = delete;

    Controller(Controller&&) = delete;

    Controller& operator=(const Controller&) = delete;

    Controller& operator=(Controller&&) = delete;

    virtual ~Controller() = default;

    /**
     * @brief Handles a game event.
     * @details This method is primarily used by player-controlled strategies to react to keyboard input or other
     * relevant events.
     * @param event The event to be processed.
     */
    virtual void handle(const Event& event) = 0;

    /**
     * @brief Updates the state of the controlled paddle.
     *
     * This method is called on every frame and contains the core logic for the controller's behavior (e.g., moving the
     * paddle based on AI logic).
     * @param paddle The paddle to be controlled.
     * @param table A constant reference to the game table.
     * @param ball A constant reference to the ball.
     * @param dt The time elapsed since the last update frame (delta time) in seconds.
     */
    virtual void update(Paddle& paddle, const Table& table, const Ball& ball, Seconds dt) = 0;
};

} // namespace pong
