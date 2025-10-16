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

#include "Paddle.hpp"
#include "Controller.hpp"
#include "Renderer.hpp"
#include "Event.hpp"
#include "Scene.hpp"
#include "Table.hpp"

namespace pong {

Paddle::Paddle(std::unique_ptr<Controller> controller, const glm::vec2& position, const glm::vec2& size)
    :
    Entity       (Type::Paddle),
    mController  (std::move(controller)),
    mPosition    (position),
    mPositionPrev(position),
    mSize        (size)
{}

void Paddle::setPosition(const glm::vec2 &position)
{
    mPosition     = position;
    mPositionPrev = position;
}

void Paddle::moveUp()
{
    mSpeed = MovementSpeed;
}

void Paddle::moveDown()
{
    mSpeed = -MovementSpeed;
}

void Paddle::stop()
{
    mSpeed = 0.0f;
}

void Paddle::setup(const Table& table, const Ball& ball)
{
    mTable = &table;
    mBall  = &ball;
}

void Paddle::handle(const Event& event)
{
    mController->handle(event);

    if (event.type() == Event::Type::Pause)
    {
        // If game is paused reset the position to avoid interpolation.
        mPositionPrev = mPosition;
    }
}

void Paddle::update(const Seconds dt)
{
    mController->update(*this, *mTable, *mBall, dt);

    mPositionPrev = mPosition;
    mPosition     = mPosition + glm::vec2(0.0f, mSpeed * dt.count());

    if (mPosition.y + mSize.y * 0.5f > mTable->top())    { mPosition.y = mTable->top()    - mSize.y * 0.5f; }
    if (mPosition.y - mSize.y * 0.5f < mTable->bottom()) { mPosition.y = mTable->bottom() + mSize.y * 0.5f; }
}

void Paddle::draw(Renderer& renderer, const float interp)
{
    renderer.queueQuad(mPosition * interp + mPositionPrev * (1.0f - interp), mSize);
}

} // namespace pong
