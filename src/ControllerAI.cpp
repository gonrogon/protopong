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

#include "ControllerAI.hpp"
#include "Table.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include <glm/gtc/random.hpp>

namespace pong {

void ControllerAI::update(Paddle& paddle, const Table& table, const Ball& ball, const Seconds dt)
{
    // On the very first update, set the initial target to the table's center.
    if (mFirst)
    {
        mTarget = table.position().y;
        mFirst  = false;
    }
    // Accumulate time since the last major logic update.
    mTimeSinceTargetUpdate += dt;
    // If enough time has elapsed since the last update, the target is recalculated.
    if (mTimeSinceTargetUpdate > TargetUpdateInterval)
    {
        mTimeSinceTargetUpdate = Seconds::zero();
        updateTarget(paddle, table, ball);
    }
    // Move the paddle towards the current target in every frame.
    moveTowardsTarget(paddle);
}

void ControllerAI::updateTarget(Paddle& paddle, const Table& table, const Ball& ball)
{
    bool isBallIncoming = false;
    if (paddle.position().x < 0)
    {
        isBallIncoming = ball.speed().x < 0;
    }
    else
    {
        isBallIncoming = ball.speed().x > 0;
    }

    if (isBallIncoming) {
        // The paddle is no longer in "return to center" mode.
        mBack = false;
        // Predict where the ball will be on the Y-axis when it reaches the paddle.
        const float timeToImpact = (paddle.position().x - ball.position().x) / ball.speed().x;
        const float predictedY = ball.position().y + ball.speed().y * timeToImpact;
        // Only update the target if the new prediction is significantly different.
        if (std::abs(predictedY - mTarget) > TargetDeadZone)
        {
            // Add some random error to make the AI feel more human.
            const float error = paddle.size().y * (HitPositionBase + glm::linearRand(-HitPositionError, HitPositionError));

            if (predictedY < paddle.position().y)
            {
                mTarget = predictedY + error;
            }
            else
            {
                mTarget = predictedY - error;
            }
        }
    }
    else
    {
        if (!mBack)
        {
            const float error = table.size().y * ReturnPositionErrorFactor;
            mTarget = table.position().y + glm::linearRand(-error, error);
            mBack   = true;
        }
    }
}

void ControllerAI::moveTowardsTarget(Paddle& paddle)
{
    // Move towards the target, but stop if we are within the dead zone to prevent jitter.
    if (std::abs(paddle.position().y - mTarget) > TargetDeadZone)
    {
        if (paddle.position().y < mTarget)
        {
            paddle.moveUp();
        }
        else
        {
            paddle.moveDown();
        }
    }
    else
    {
        paddle.stop();
    }
}

} // namespace pong
