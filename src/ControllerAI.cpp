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
/// @file   src/ControllerAI.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "ControllerAI.hpp"
#include "Table.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

void ControllerAI::update(Paddle& paddle, const Table& table, const Ball& ball, const float dt)
{
    // First update, go to the center of the table.
    if (mFirst)
    {
        mTarget = table.position().y;
        mFirst  = false;
    }
    // If enough time has elapsed since the last update, the target is recalculated.
    if (mTime > 0.3f)
    {
        mTime = 0.0f;
        // Calculate the target position in order to hit the ball when it reaches the paddle.
        if (ball.speed().x < 0)
        {
            mBack   = false;
            float t = (paddle.position().x - ball.position().x) / ball.speed().x;
            float y = ball.position().y + ball.speed().y * t;
            // Update the target (with some error) only if the paddle is not already on it (with some error).
            if (y < mTarget - 1.0f || y > mTarget + 1.0f )
            {
                if (y < paddle.position().y)
                {
                    mTarget = y + paddle.size().y * (0.40f + glm::linearRand(-0.2f, 0.2f));
                }
                else
                {
                    mTarget = y - paddle.size().y * (0.40f + glm::linearRand(-0.2f, 0.2f));
                }
            }
        }
        // If the ball moves away and the paddle is not moving to the center of the table, recalculate the target
        // position to fall back to the center of the table (with some error).
        else
        {
            if (!mBack)
            {
                mTarget = table.position().y + glm::linearRand(-table.size().y * 0.1f, table.size().y * 0.1f);
                mBack   = true;
            }
        }
    }
    // Accumulated time.
    mTime += dt;
    // Move to the target.
    if (paddle.position().y < mTarget - 1.0f || paddle.position().y > mTarget + 1.0f)
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

////////////////////////////////////////////////////////////

} // namespace pong
