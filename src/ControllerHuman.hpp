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

#include "Controller.hpp"

namespace pong {

/**
 * @brief Implements a configurable controller strategy for a human-controlled paddle.
 *
 * This controller is configured for a specific player (A or B) upon construction. It internally filters game events to
 * respond only to the inputs designated for its player.
 */
class ControllerHuman : public Controller
{
public:

    /**
     * @brief Defines an enumeration that identifies the player this controller should respond to.
     */
    enum class Player { A, B };

    /**
     * @brief Constructor.
     * @param player The player (A or B) that this controller will listen to.
     */
    explicit ControllerHuman(Player player) noexcept;

    void handle(const Event& event) override;

    void update(Paddle& paddle, const Table& table, const Ball& ball, float dt) override;

private:

    /** @brief @brief The player this controller is responsible for. */
    Player mPlayer;

    /** @brief A state counter indicating the desired direction (-1 for down, 0 for stop, 1 for up). */
    int mMoveDirection = 0;
};

////////////////////////////////////////////////////////////

} // namespace pong
