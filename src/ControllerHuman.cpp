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
/// @file   src/ControllerHuman.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "ControllerHuman.hpp"
#include "Event.hpp"
#include "Paddle.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

void ControllerHuman::handle(const Event& event)
{
    switch (event.type())
    {
        case Event::Type::PlayerAMoveUp:
        case Event::Type::PlayerBMoveUp:
        {
            mSpeed += 1;
        }
        break;

        case Event::Type::PlayerAMoveUpReleased:
        case Event::Type::PlayerBMoveUpReleased:
        {
            mSpeed -= 1;
        }
        break;

        case Event::Type::PlayerAMoveDown:
        case Event::Type::PlayerBMoveDown:
        {
            mSpeed -= 1;
        }
        break;

        case Event::Type::PlayerAMoveDownReleased:
        case Event::Type::PlayerBMoveDownReleased:
        {
            mSpeed += 1;
        }
        break;

        default:break;
    }
}

////////////////////////////////////////////////////////////

void ControllerHuman::update(Paddle& paddle, const Table& table, const Ball& ball, const float dt)
{
    if (mSpeed > 0) { paddle.moveUp();   } else
    if (mSpeed < 0) { paddle.moveDown(); } else
    {
        paddle.stop();
    }
}

////////////////////////////////////////////////////////////

} // namespace pong
