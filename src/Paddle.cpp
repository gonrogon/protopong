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
/// @file   src/Paddle.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Paddle.hpp"
#include "Controller.hpp"
#include "Event.hpp"
#include "Scene.hpp"
#include "Table.hpp"
#include "Ball.hpp"
#include "RendererGL3.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

Paddle::Paddle(Controller* controller, const glm::vec2& position, const glm::vec2& size)
    :
    Entity       (Type::Paddle),
    mController  (controller),
    mPosition    (position),
    mPositionPrev(position),
    mSize        (size)
{}

////////////////////////////////////////////////////////////

const glm::vec2& Paddle::size() const
{
    return mSize;
}

////////////////////////////////////////////////////////////

const glm::vec2& Paddle::position() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////

float Paddle::top() const
{
    return mPosition.y + mSize.y;
}

////////////////////////////////////////////////////////////

float Paddle::bottom() const
{
    return mPosition.y - mSize.y;
}

////////////////////////////////////////////////////////////

float Paddle::speed() const
{
    return mSpeed;
}

////////////////////////////////////////////////////////////

void Paddle::moveUp()
{
    mSpeed = 120.0f;
}

////////////////////////////////////////////////////////////

void Paddle::moveDown()
{
    mSpeed = -120.0f;
}

////////////////////////////////////////////////////////////

void Paddle::stop()
{
    mSpeed = 0.0f;
}

////////////////////////////////////////////////////////////

void Paddle::setup(const int table, const int ball)
{
    mTable = table;
    mBall  = ball;
}

////////////////////////////////////////////////////////////

void Paddle::handle(const Event& event)
{
    mController->handle(event);

    if (event.type() == Event::Type::Pause)
    {
        // If game is paused reset the position to avoid interpolation.
        mPositionPrev = mPosition;
    }
}

////////////////////////////////////////////////////////////

void Paddle::update(const float dt)
{
    Table& table = static_cast<Table&>(scene().at(mTable));
    Ball&  ball  = static_cast<Ball&> (scene().at(mBall));

    mController->update(*this, table, ball, dt);

    mPositionPrev = mPosition;
    mPosition     = mPosition + glm::vec2(0.0f, mSpeed * dt);

    if (mPosition.y + mSize.y * 0.5f > table.top())    { mPosition.y = table.top()    - mSize.y * 0.5f; }
    if (mPosition.y - mSize.y * 0.5f < table.bottom()) { mPosition.y = table.bottom() + mSize.y * 0.5f; }
}

////////////////////////////////////////////////////////////

void Paddle::draw(const float dt, const float interp, Renderer& renderer)
{
    renderer.queueQuad(mPosition * interp + mPositionPrev * (1.0f - interp), mSize);
}

////////////////////////////////////////////////////////////

} // namespace pong
