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
 * @brief Implements a controller strategy for an AI-controlled paddle.
 *
 * This AI features several human-like behaviors:
 * - **Predictive Tracking:** It calculates the ball's future trajectory to intercept it.
 * - **Delayed Reaction:** It only updates its target periodically, not every frame, to avoid jittery, robotic movement.
 * - **Intentional Error:** It adds a slight random offset to its target position to make its hits less predictable and
 *   perfect.
 */
class ControllerAI final : public Controller
{
public:

    enum class Side { Left, Right };

    /** @brief How often the AI re-evaluates its target, in seconds. Lower is harder. */
    constexpr static TimeDuration TargetUpdateInterval = std::chrono::milliseconds(300);

    /** @brief The distance from the target at which the paddle stops moving. Prevents oscillation. */
    constexpr static float TargetDeadZone = 1.0f;

    /** @brief The base offset from the paddle's center to hit the ball. */
    constexpr static float HitPositionBase = 0.40f;

    /** @brief The random error range added to the hit position. */
    constexpr static float HitPositionError = 0.2f;

    /** @brief The random error range when returning to the center, as a factor of table height. */
    constexpr static float ReturnPositionErrorFactor = 0.1f;

    /**
     * @brief Constructor.
     */
    /**
     * @brief Constructor.
     * should react to. Typically -1.0f for the left paddle, 1.0f for the right.
     */
    ControllerAI() = default;

    /**
     * @brief AI does not react to direct user events, so this is empty.
     */
    void handle(const Event& event) override {}

    void update(Paddle& paddle, const Table& table, const Ball& ball, TimeDuration dt) override;

private:
    /**
     * @brief Recalculates the AI's target Y-coordinate based on the ball's trajectory.
     * @param paddle Paddle being controlled.
     * @param table Game table.
     * @param ball Game ball.
     */
    void updateTarget(Paddle& paddle, const Table& table, const Ball& ball);

    /**
     * @brief Moves the paddle towards the current target Y-coordinate.
     * @param paddle Paddle being controlled.
     */
    void moveTowardsTarget(Paddle& paddle);

private:

    /** @brief Flag to handle the first update frame uniquely. */
    bool mFirst = true;

    /** @brief Flag to ensure the "return to center" target is calculated only once. */
    bool mBack = false;

    /** @brief The target Y-coordinate the paddle is currently trying to reach. */
    float mTarget = 0.0f;

    /**
     * @brief Time elapsed since the last update of the target, it is used to control how often the target is
     * recalculated.
     */
    TimeDuration mTimeSinceTargetUpdate = {};
};

} // namespace pong
