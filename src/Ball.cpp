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
/// @file   src/Ball.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Ball.hpp"
#include "Audio.hpp"
#include "Event.hpp"
#include "Scene.hpp"
#include "Table.hpp"
#include "Paddle.hpp"
#include "Game.hpp"
#include "RendererGL3.hpp"

////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////

namespace pong {
namespace      {

////////////////////////////////////////////////////////////

/**
 * @brief Check if a ball collides with a paddle.
 *
 * @param ball   Ball.
 * @param paddle Paddle.
 * @param where  Point where the collision happens.
 *
 * @return True if the ball and the paddle collide; otherwise, false.
 */
bool collision(const Ball& ball, const Paddle& paddle, glm::vec2& where);

/**
 * @brief Calculate the intersection between a circle and a line.
 *
 * @param c Center of the circle.
 * @param r Radius of the circle.
 * @param a Start point of the line.
 * @param b End point of the line.
 * @param p Intersection.
 *
 * @return True if the circle and the line intersect; otherwise, false.
 */
bool collisionCircleLine(const glm::vec2& c, float r, const glm::vec2& a, const glm::vec2& b, glm::vec2& p);

////////////////////////////////////////////////////////////

} // namespace

////////////////////////////////////////////////////////////
// Ball
////////////////////////////////////////////////////////////

const float Ball::MaxBounceAngle = glm::radians(55.0f);
const float Ball::MinSpeed       = 100.0f;
const float Ball::MaxSpeed       = 180.0f;

////////////////////////////////////////////////////////////

Ball::Ball(const glm::vec2& position, const float radius)
    :
    Entity       (Type::Ball),
    mPosition    (position),
    mPositionPrev(position),
    mRadius      (radius),
    mSpeed       (100.0f, 0.0f)
{}

////////////////////////////////////////////////////////////

const glm::vec2& Ball::position() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////

float Ball::radius() const
{
    return mRadius;
}

////////////////////////////////////////////////////////////

float Ball::left() const
{
    return mPosition.x - mRadius;
}

////////////////////////////////////////////////////////////

float Ball::right() const
{
    return mPosition.x + mRadius;
}

////////////////////////////////////////////////////////////

float Ball::top() const
{
    return mPosition.y + mRadius;
}

////////////////////////////////////////////////////////////

float Ball::bottom() const
{
    return mPosition.y - mRadius;
}

////////////////////////////////////////////////////////////

const glm::vec2& Ball::speed() const
{
    return mSpeed;
}

////////////////////////////////////////////////////////////

bool Ball::point() const
{
    return mPoint == Point::A || mPoint == Point::B;
}

////////////////////////////////////////////////////////////

bool Ball::pointPaddleA() const
{
    return mPoint == Point::A;
}

////////////////////////////////////////////////////////////

bool Ball::pointPaddleB() const
{
    return mPoint == Point::B;
}

////////////////////////////////////////////////////////////

void Ball::reset(const glm::vec2& position, const float speed)
{
    mPosition     = position;
    mPositionPrev = position;
    mSpeed        = glm::vec2(speed, 0.0f);
    mPoint        = Point::None;
}

////////////////////////////////////////////////////////////

void Ball::setup(const int table, const int paddleA, const int paddleB)
{
    mTable   = table;
    mPaddleA = paddleA;
    mPaddleB = paddleB;
}

////////////////////////////////////////////////////////////

void Ball::handle(const Event& event)
{
    // If the game is paused, the position is reset to avoid the interpolation.
    if (event.is(Event::Type::Pause))
    {
        mPositionPrev = mPosition;
    }
}

////////////////////////////////////////////////////////////

void Ball::update(const float dt)
{
    auto& table   = static_cast<Table&> (scene().at(mTable));
    auto& paddleA = static_cast<Paddle&>(scene().at(mPaddleA));
    auto& paddleB = static_cast<Paddle&>(scene().at(mPaddleB));
    // Reset the collision flag and update the position.
    mCollision     = false;
    mPositionPrev  = mPosition;
    mPosition      = mPosition + mSpeed * dt;
    // Check for collisions with the boundaries of the table.
    if (top()    > table.top())    { mPosition.y = table.top()    - mRadius; mSpeed.y *= -1.0f; mCollision = true; }
    if (bottom() < table.bottom()) { mPosition.y = table.bottom() + mRadius; mSpeed.y *= -1.0f; mCollision = true; }
    if (right()  > table.right())  { mPoint = Point::B; }
    if (left()   < table.left ())  { mPoint = Point::A; }
    // Check for collisions with the paddles.
    glm::vec2 pos;
    bool ca = collision(*this, paddleA, pos);
    bool cb = collision(*this, paddleB, pos);
    // If there was a collision with the paddles.
    if (ca || cb)
    {
        float rdist;
        float speed = glm::length(mSpeed);
        float sign  = glm::sign(mSpeed.x);
        // There was a collision, so play the collision sound.
        mCollision = true;
        // Collision with paddle A.
        if (ca)
        {
            mPosition.x = paddleA.position().x - paddleA.size().x * 0.5f - mRadius;
            // Calculate the position of the collision relative to the center of the paddle A.
            rdist = (pos.y - paddleA.position().y) / (paddleA.size().y * 0.5f);
        }
        // Collision with paddle B.
        if (cb)
        {
            mPosition.x = paddleB.position().x + paddleB.size().x * 0.5f + mRadius;
            // Calculate the position of the collision relative to the center of the paddle B.
            rdist = (pos.y - paddleB.position().y) / (paddleB.size().y * 0.5f);
        }
        // Increment the speed regarding the position of the collision (near the center, the speed is reduced; near
        // the end of the paddle, the speed is increased).
        speed += 25.0f * ((3.0f * glm::abs(rdist) - 1) * (2.0f - glm::abs(rdist)) * 0.5f);
        // Clamp the speed between its minimum and maximum values.
        speed = glm::min(MaxSpeed, glm::max(MinSpeed, speed));
        // Calculate the new speed vector.
        mSpeed = -speed * glm::rotate(glm::vec2(sign, 0.0f), -sign * MaxBounceAngle * rdist);
    }
    // Play the collision sound if there was a collision.
    if (mCollision)
    {
        scene().game().audio().play();
    }
}

////////////////////////////////////////////////////////////

void Ball::draw(const float dt, const float interp, Renderer& renderer)
{
    if (mPoint != Point::None)
    {
        return;
    }

    renderer.queueQuad(mPosition * interp + mPositionPrev * (1.0f - interp), glm::vec2(mRadius * 2.0f));
}

////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////

bool collision(const Ball& ball, const Paddle& paddle, glm::vec2& where)
{
    // Add the ball radius to the paddle radius.
    const float radii = glm::length(paddle.size() * 0.5f) + ball.radius();
    // Check if the paddle is close enough to collide with the ball. This calculation is faster than circle/segment
    // intersection and, if there is a collision, it is limited to the segment of the paddle.
    if (glm::length2(paddle.position() - ball.position()) > (radii * radii))
    {
        return false;
    }
    // Calculate the intersection between the ball and the paddle front line.
    if (collisionCircleLine(ball.position(), ball.radius(),
        {paddle.position().x - paddle.size().x * 0.5f, paddle.position().y - paddle.size().y * 0.5f},
        {paddle.position().x - paddle.size().x * 0.5f, paddle.position().y + paddle.size().y * 0.5f},
        where
    ))
    {
        return true;
    }
    // Calculate the intersection between the ball and the paddle front line.
    if (collisionCircleLine(ball.position(), ball.radius(),
        {paddle.position().x + paddle.size().x * 0.5f, paddle.position().y - paddle.size().y * 0.5f},
        {paddle.position().x + paddle.size().x * 0.5f, paddle.position().y + paddle.size().y * 0.5f},
        where
    ))
    {
        return true;
    }
    // No collision.
    return false;
}

////////////////////////////////////////////////////////////

bool collisionCircleLine(const glm::vec2& c, const float r, const glm::vec2& a, const glm::vec2& b, glm::vec2& p)
{
    const glm::vec2 ac = c - a;
    const glm::vec2 ab = b - a;
    // Calculate the square length of the vectors and dot product.
    const float dot    = glm::dot(ac, ab);
    const float len2AC = glm::length2(ac);
    const float len2AB = glm::length2(ab);
    // Calculate the discriminant.
    const float discriminant = dot * dot - len2AB * (len2AC - (r * r));
    // If the discriminant is greater than or equal to zero, we have real solutions (there is an intersection).
    if (discriminant >= 0)
    {
        const float sqrt = glm::sqrt(discriminant);
        const float inv  = 1.0f / len2AB;
        const float r0   = glm::pow(-dot + sqrt, 2.0f) * inv * inv;
        const float r1   = glm::pow(-dot - sqrt, 2.0f) * inv * inv;
        // Check if the solutions are onto the segment.
        if ((r0 >= 0 && r0 <= 1) || (r1 >= 0 && r1 <= 1))
        {
            // For this case, the exact intersection points are not needed, so the nearest point to the center
            // of the ball is calculated (projection of the vector ac onto ab).
            p = a + ab * dot * inv;
            // Done.
            return true;
        }
    }
    // No intersection.
    return false;
}

////////////////////////////////////////////////////////////

} // namespace
} // namespace pong
