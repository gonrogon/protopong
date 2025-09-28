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

#include "Entity.hpp"
#include <glm/glm.hpp>

namespace pong {

class Table;
class Paddle;

/**
 * @brief Represents the game ball, handling its physics, collisions, and scoring logic.
 *
 * The Ball is the central dynamic entity in the game. It moves autonomously and interacts with the paddles and table
 * boundaries. It requires being linked to the Table and Paddles via the `setup()` method after its construction to
 * function correctly. The class encapsulates all physics calculations, including complex angular bounces off the
 * paddles.
 */
class Ball final : public Entity
{
    /** @brief Maximum angle (in radians) for a bounce off a paddle. */
    static constexpr float MaxBounceAngle = glm::radians(55.0f);

    /** @brief Minimum speed of the ball. */
    static constexpr float MinSpeed = 100.0f;

    /** @brief Maximum speed of the ball. */
    static constexpr float MaxSpeed = 180.0f;

    /** @brief Defines which player (if any) scored a point on the last update. */
    enum class Point
    {
        None, //!< None.
        A,    //!< A point was scored for Player A.
        B     //!< A point was scored for Player B.
    };

public:

    /**
     * @brief Constructor.
     * @param position The initial center position.
     * @param radius The radius of the ball.
     */
    Ball(const glm::vec2& position, float radius);

    /**
     * @brief Gets the current position of the ball.
     * @return Position.
     */
    [[nodiscard]] const glm::vec2& position() const { return mPosition; }

    /**
     * @brief Gets the radius of the ball (size).
     * @return Radius.
     */
    [[nodiscard]] float radius() const { return mRadius; }

    /**
     * @return X-coordinate of the left side of the ball.
     */
    [[nodiscard]] float left() const { return mPosition.x - mRadius; }

    /**
     * @return X-coordinate of the right side of the ball.
     */
    [[nodiscard]] float right() const { return mPosition.x + mRadius; }

    /**
     * @return Y-coordinate of the top of the ball.
     */
    [[nodiscard]] float top() const { return mPosition.y + mRadius; }

    /**
     * @return Y-coordinate of the bottom of the ball.
     */
    [[nodiscard]] float bottom() const { return mPosition.y - mRadius; }

    /**
     * @brief Gets current speed of the ball.
     * @return Speed.
     */
    [[nodiscard]] const glm::vec2& speed() const { return mSpeed; }

    /**
     * @brief Checks if a point was scored in the last frame.
     * @return True if any player scored, false otherwise.
     */
    [[nodiscard]] bool point() const { return mPoint == Point::A || mPoint == Point::B; }

    /**
     * @brief Checks if Player A scored in the last frame.
     * @return True if Player A scored, false otherwise.
     */
    [[nodiscard]] bool pointPaddleA() const { return mPoint == Point::A; }

    /**
     * @brief Checks if Player B scored in the last frame.
     * @return True if Player B scored, false otherwise.
     */
    [[nodiscard]] bool pointPaddleB() const { return mPoint == Point::B; }

    /**
     * @brief Resets the ball's state for a new round.
     *
     * This repositions the ball and sets its initial speed and direction.
     * @param position The new starting position.
     * @param speed The new starting speed (X-component, Y is zero).
     */
    void reset(const glm::vec2& position, float speed);

    /**
     * @brief Links the ball to its external gameplay dependencies.
     * @param table A reference to the game table.
     * @param paddleA A reference to paddle A.
     * @param paddleB A reference to paddle B.
     */
    void setup(const Table& table, const Paddle& paddleA, const Paddle& paddleB);

    void handle(const Event& event) override;

    void update(Seconds dt) override;

    void draw(Renderer& renderer, float interp) override;

private:

    /** @brief Checks for and resolves collisions with the top and bottom walls. */
    void checkWallCollisions();

    /** @brief Checks if the ball has passed the left or right boundaries, triggering a score. */
    void checkScore();

    /** @brief Checks for and resolves collisions with both paddles, calculating new bounce physics. */
    void checkPaddleCollisions();

public:

    /**
     * @brief Checks if a ball collides with a paddle.
     *
     * @param ball Ball.
     * @param paddle Paddle.
     * @param where Point where the collision happens.
     *
     * @return True if the ball and the paddle collide, false otherwise.
     */
    static bool collision(const Ball& ball, const Paddle& paddle, glm::vec2& where);

private:

    /** @brief Current center position of the ball. */
    glm::vec2 mPosition;

    /** @brief Position in the previous frame, for interpolation. */
    glm::vec2 mPositionPrev;

    /** @brief Radius (size). */
    float mRadius = 1.0f;

    /** @brief The current velocity vector of the ball. */
    glm::vec2 mSpeed;

    /** @brief The scoring state from the last update. */
    Point mPoint = Point::None;

    /** @brief Identifier of the table. */
    const Table* mTable = nullptr;

    /** @brief Identifier of the paddle A. */
    const Paddle* mPaddleA = nullptr;

    /** @brief Identifier of the paddle B. */
    const Paddle* mPaddleB = nullptr;

    /** @brief Flag indicating if there was a collision or not. */
    bool mCollisionOccurred = false;
};

} // namespace pong
