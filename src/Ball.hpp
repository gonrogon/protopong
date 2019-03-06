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
/// @file   src/Ball.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Entity.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an entity for the ball.
////////////////////////////////////////////////////////////
class Ball : public Entity
{
private:

    /** @brief Maximum bounce angle. */
    static const float MaxBounceAngle;

    /** @brief Minimum speed. */
    static const float MinSpeed;

    /** @brief Maximum speed. */
    static const float MaxSpeed;

    /**
     * @brief Defines an enumeration with the types of points.
     */
    enum class Point
    {
        None, //!< None.
        A,    //!< Point for player A.
        B     //!< Point for player B.
    };

public:

    /**
     * @brief Constructor.
     *
     * @param position Position.
     * @param radius   Radius.
     */
    Ball(const glm::vec2& position, float radius);

    /**
     * @return Position.
     */
    const glm::vec2& position() const;

    /**
     * @return Radius.
     */
    float radius() const;

    /**
     * @return X-coordinate of the left side of the ball.
     */
    float left() const;

    /**
     * @return X-coordinate of the right side of the ball.
     */
    float right() const;

    /**
     * @return Y-coordinate of the top of the ball.
     */
    float top() const;

    /**
     * @return Y-coordinate of the bottom of the ball.
     */
    float bottom() const;

    /**
     * @return Speed.
     */
    const glm::vec2& speed() const;

    /**
     * @return True if paddle A or B has scored a point; otherwise, false.
     */
    bool point() const;

    /**
     * @return True if paddle A has scored a point; otherwise, false.
     */
    bool pointPaddleA() const;

    /**
     * @return True if paddle B has scored a point; otherwise, false.
     */
    bool pointPaddleB() const;

    /**
     * @brief Reset the ball.
     *
     * @param position Initial position.
     * @param speed    Initial speed.
     */
    void reset(const glm::vec2& position, float speed);

    /**
     * @brief Set up the ball.
     *
     * @param table   Identifier of the table.
     * @param paddleA Identifier of the paddle A.
     * @param paddleB Identifier of the paddle B.
     */
    void setup(int table, int paddleA, int paddleB);

    void handle(const Event& event) override;

    void update(float dt) override;

    void draw(float dt, float interp, Renderer& renderer) override;

private:

    /** @brief Position. */
    glm::vec2 mPosition;

    /** @brief Previous position. */
    glm::vec2 mPositionPrev;

    /** @brief Radius (size). */
    float mRadius = 1.0f;

    /** @brief Speed. */
    glm::vec2 mSpeed;

    /** @brief Point type. */
    Point mPoint = Point::None;

    /** @brief Identifier of the table. */
    int mTable = -1;

    /** @brief Identifier of the paddle A. */
    int mPaddleA = -1;

    /** @brief Identifier of the paddle B. */
    int mPaddleB = -1;

    /** @brief Flag indicating if there was a collision or not. */
    bool mCollision = false;
};

////////////////////////////////////////////////////////////

} // namespace pong
