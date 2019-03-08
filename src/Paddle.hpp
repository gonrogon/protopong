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
/// @file   src/Paddle.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Entity.hpp"
#include <memory>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an entity for the paddles.
////////////////////////////////////////////////////////////
class Paddle : public Entity
{
private:

    /** @brief Define a type for a unique pointer to controllers. */
    typedef std::unique_ptr<Controller> ControllerPtr;

public:

    /**
     * @brief Constructor.
     *
     * @param controller Controller.
     * @param position   Initial position.
     * @param size       Size.
     */
    Paddle(Controller* controller, const glm::vec2& position, const glm::vec2& size);

    /**
     * @return Position.
     */
    const glm::vec2& position() const { return mPosition; }

    /**
     * @brief Set the position.
     *
     * @param position Position.
     */
    void setPosition(const glm::vec2& position);

    /**
     * @return Size.
     */
    const glm::vec2& size() const { return mSize; }

    /**
     * @brief Set the speed of the paddle to move up.
     */
    void moveUp();

    /**
     * @brief Set the speed of the paddle to move down.
     */
    void moveDown();

    /**
     * @brief Set the speed of the paddle to zero.
     */
    void stop();

    /**
     * @brief Set up the paddle.
     *
     * @param table Identifier of the table.
     * @param ball  Identifier of the ball.
     */
    void setup(int table, int ball);

    void handle(const Event& event) override;

    void update(float dt) override;

    void draw(float dt, float interp, Renderer& renderer) override;

private:

    /** @brief Controller. */
    ControllerPtr mController;

    /** @brief Position. */
    glm::vec2 mPosition;

    /** @brief Previous position. */
    glm::vec2 mPositionPrev;

    /** @brief Size (x = width, y = height). */
    glm::vec2 mSize = glm::vec2(1.0f);

    /** @brief Speed. */
    float mSpeed = 0.0f;

    /** @brief Identifier of the table. */
    int mTable = -1;

    /** @brief Identifier of the ball. */
    int mBall = -1;
};

////////////////////////////////////////////////////////////

} // namespace pong
