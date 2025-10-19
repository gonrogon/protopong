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

#include "Ball.hpp"
#include "Audio.hpp"
#include "Event.hpp"
#include "Scene.hpp"
#include "Table.hpp"
#include "Paddle.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>

namespace pong {
namespace      {

/**
 * @brief Calculates the intersection between a circle and a line.
 *
 * @param c Center of the circle.
 * @param r Radius of the circle.
 * @param a Start point of the line.
 * @param b End point of the line.
 * @param p Intersection.
 *
 * @return True if the circle and the line intersect, false otherwise.
 */
bool collisionCircleLine(const glm::vec2& c, float r, const glm::vec2& a, const glm::vec2& b, glm::vec2& p);

} // namespace

Ball::Ball(const glm::vec2& position, const float radius)
    :
    Entity       (Type::Ball),
    mPosition    (position),
    mPositionPrev(position),
    mRadius      (radius),
    mSpeed       (100.0f, 0.0f)
{}

void Ball::reset(const glm::vec2& position, const float speed)
{
    mPosition     = position;
    mPositionPrev = position;
    mSpeed        = glm::vec2(speed, 0.0f);
    mPoint        = Point::None;
}

void Ball::setup(const Table& table, const Paddle& paddleA, const Paddle& paddleB)
{
    mTable   = &table;
    mPaddleA = &paddleA;
    mPaddleB = &paddleB;
}

void Ball::handle(const Event& event)
{
    // If the game is paused, the position is reset to avoid the interpolation.
    if (event.is(Event::Type::Pause))
    {
        mPositionPrev = mPosition;
    }
}

void Ball::update(const TimeDuration dt)
{
    if (!mTable || !mPaddleA || !mPaddleB)
    {
        return;
    }
    // Reset the collision state and update the position.
    mCollisionOccurred = false;
    mPositionPrev = mPosition;
    mPosition = mPosition + mSpeed * static_cast<float>(dt.count());
    // Check for collisions with the top and bottom boundaries of the table.
    checkWallCollisions();
    // Check for collisions (scores) with the left and right boundaries of the table.
    checkScore();
    // Check for collisions with the paddles.
    checkPaddleCollisions();
    // Play the collision sound if there was a collision.
    if (mCollisionOccurred)
    {
        scene()->game().audio().play();
    }
}

void Ball::checkWallCollisions()
{
    if (top() > mTable->top())
    {
        mPosition.y = mTable->top() - mRadius;
        mSpeed.y *= -1.0f;
        mCollisionOccurred = true;
    }

    if (bottom() < mTable->bottom())
    {
        mPosition.y = mTable->bottom() + mRadius;
        mSpeed.y *= -1.0f;
        mCollisionOccurred = true;
    }
}

void Ball::checkScore()
{
    if (right() > mTable->right())
    {
        mPoint = Point::B;
    }

    if (left() < mTable->left ())
    {
        mPoint = Point::A;
    }
}

void Ball::checkPaddleCollisions()
{
    // Check for collisions with the paddles.
    glm::vec2 pos;
    const bool ca = collision(*this, *mPaddleA, pos);
    const bool cb = collision(*this, *mPaddleB, pos);
    // If there was a collision with the paddles.
    if (!ca && !cb)
    {
        return;
    }

    float       rDist = 0.0f; // Relative distance.
    float       speed = glm::length(mSpeed);
    const float sign  = glm::sign(mSpeed.x);
    // There was a collision, so play the collision sound.
    mCollisionOccurred = true;
    // Collision with paddle A.
    if (ca)
    {
        mPosition.x = mPaddleA->position().x - mPaddleA->size().x * 0.5f - mRadius;
        // Calculate the position of the collision relative to the center of the paddle A.
        rDist = (pos.y - mPaddleA->position().y) / (mPaddleA->size().y * 0.5f);
    }
    // Collision with paddle B.
    if (cb)
    {
        mPosition.x = mPaddleB->position().x + mPaddleB->size().x * 0.5f + mRadius;
        // Calculate the position of the collision relative to the center of the paddle B.
        rDist = (pos.y - mPaddleB->position().y) / (mPaddleB->size().y * 0.5f);
    }
    // Increment the speed regarding the position of the collision (near the center, the speed is reduced; near the end
    // of the paddle, the speed is increased).
    speed += 25.0f * ((3.0f * glm::abs(rDist) - 1) * (2.0f - glm::abs(rDist)) * 0.5f);
    // Clamp the speed between its minimum and maximum values.
    speed = glm::min(MaxSpeed, glm::max(MinSpeed, speed));
    // Calculate the new speed vector.
    mSpeed = -speed * glm::rotate(glm::vec2(sign, 0.0f), -sign * MaxBounceAngle * rDist);
}

void Ball::draw(Renderer& renderer, const float interp)
{
    if (mPoint != Point::None)
    {
        return;
    }

    renderer.queueQuad(mPosition * interp + mPositionPrev * (1.0f - interp), glm::vec2(mRadius * 2.0f));
}

bool Ball::collision(const Ball& ball, const Paddle& paddle, glm::vec2& where)
{
    // Add the ball radius to the paddle radius.
    const float radii = glm::length(paddle.size() * 0.5f) + ball.radius();
    // Check if the paddle is close enough to collide with the ball. This calculation is faster than circle/segment
    // intersection and if there is a collision, it is limited to the segment of the paddle.
    if (glm::length2(paddle.position() - ball.position()) > (radii * radii))
    {
        return false;
    }
    // Calculate the intersection between the ball and the paddle front line.
    if (collisionCircleLine(ball.position(), ball.radius(),
            {paddle.position().x - paddle.size().x * 0.5f, paddle.position().y - paddle.size().y * 0.5f},
            {paddle.position().x - paddle.size().x * 0.5f, paddle.position().y + paddle.size().y * 0.5f},
            where))
    {
        return true;
    }
    // Calculate the intersection between the ball and the paddle back line.
    if (collisionCircleLine(ball.position(), ball.radius(),
            {paddle.position().x + paddle.size().x * 0.5f, paddle.position().y - paddle.size().y * 0.5f},
            {paddle.position().x + paddle.size().x * 0.5f, paddle.position().y + paddle.size().y * 0.5f},
            where))
    {
        return true;
    }
    // No collision.
    return false;
}

namespace {

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

} // namespace
} // namespace pong
