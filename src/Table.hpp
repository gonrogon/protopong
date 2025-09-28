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

/**
 * @brief Defines the game table, including its boundaries and visual representation.
 *
 * This entity is typically static and provides the limits for gameplay. It renders itself as a border and a center
 * line.
 */
class Table final : public Entity
{
public:

    /**
     * @brief Constructor.
     * @param position The center position of the table.
     * @param size The total width and height of the table.
     */
    Table(const glm::vec2& position, const glm::vec2& size) noexcept;

    /**
     * @return Position.
     */
    [[nodiscard]] const glm::vec2& position() const noexcept { return mPosition; }

    /**
     * @return Size.
     */
    [[nodiscard]] const glm::vec2& size() const noexcept { return mSize; }

    /**
     * @return Left limit.
     */
    [[nodiscard]] float left() const noexcept { return mPosition.x - mSize.x * 0.5f; }

    /**
     * @return Right limit.
     */
    [[nodiscard]] float right() const noexcept { return mPosition.x + mSize.x * 0.5f; }

    /**
     * @return Top limit.
     */
    [[nodiscard]] float top() const noexcept { return mPosition.y + mSize.y * 0.5f; }

    /**
     * @return Bottom limit.
     */
    [[nodiscard]] float bottom() const noexcept { return mPosition.y - mSize.y * 0.5f; }

    void draw(Renderer& renderer, float interp) override;

private:

    /** @brief Position. */
    glm::vec2 mPosition;

    /** @brief Size. */
    glm::vec2 mSize;
};

////////////////////////////////////////////////////////////

} // namespace pong
