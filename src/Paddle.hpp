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
#include <memory>

namespace pong {

class Controller;
class Table;
class Ball;

/**
 * @brief Represents a player's paddle entity.
 *
 * The Paddle is a core gameplay entity that moves vertically to hit the ball.
 * Its behavior is determined by a `Controller` strategy, which can be either a human player or an AI. The Paddle must
 * be linked to the `Table` and `Ball` after construction via the `setup()` method to resolve dependencies.
 */
class Paddle final : public Entity
{
public:

    constexpr static float MovementSpeed = 120.0f;

    /**
     * @brief Constructor.
     *
     * The paddle is created in a "disconnected" state. The `setup()` method must be called after creation to link it to
     * other game entities.
     * @param controller The control strategy (e.g., player or AI). Ownership is transferred to the paddle.
     * @param position Initial center position of the paddle.
     * @param size Width and height of the paddle.
     */
    Paddle(std::unique_ptr<Controller> controller, const glm::vec2& position, const glm::vec2& size);

    /**
     * @brief Gets the current center position of the paddle.
     * @return Position.
     */
    [[nodiscard]] const glm::vec2& position() const noexcept { return mPosition; }

    /**
     * @brief Sets the position of the paddle.
     * @param position Position.
     */
    void setPosition(const glm::vec2& position);

    /**
     * @brief Gets the size of the paddle.
     * @return Size.
     */
    [[nodiscard]] const glm::vec2& size() const noexcept { return mSize; }

    /**
     * @brief Sets the speed of the paddle to move upwards.
     */
    void moveUp();

    /**
     * @brief Sets the speed of the paddle to move downwards.
     */
    void moveDown();

    /**
     * @brief Sets the paddle's vertical speed to zero.
     */
    void stop();

    /**
     * @brief Links the paddle to its external gameplay dependencies.
     *
     * This method must be called after all game entities have been created to resolve the circular dependency between
     * paddles and the ball.
     * @param table A reference to the game table for boundary checks.
     * @param ball A reference to the ball.
     */
    void setup(const Table& table, const Ball& ball);

    void handle(const Event& event) override;

    void update(TimeDuration dt) override;

    void draw(Renderer& renderer, float interp) override;

private:

    /** @brief Controller. */
    std::unique_ptr<Controller> mController;

    /** @brief The current center position of the paddle. */
    glm::vec2 mPosition;

    /** @brief The position of the paddle in the previous frame, used for interpolation. */
    glm::vec2 mPositionPrev;

    /** @brief The width and height of the paddle (x = width, y = height). */
    glm::vec2 mSize = glm::vec2(1.0f);

    /** @brief The current vertical speed of the paddle. */
    float mSpeed = 0.0f;

    /** @brief Pointer to the game table. */
    const Table* mTable = nullptr;

    /** @brief Pointer to the game ball. */
    const Ball* mBall = nullptr;
};

////////////////////////////////////////////////////////////

} // namespace pong
