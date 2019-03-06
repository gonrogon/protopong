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
/// @file   src/Table.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Entity.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an entity for the table.
////////////////////////////////////////////////////////////
class Table : public Entity
{
public:

    /**
     * @brief Constructor.
     *
     * @param position Position.
     * @param size     Size.
     */
    Table(const glm::vec2& position, const glm::vec2& size);

    /**
     * @return Position.
     */
    const glm::vec2& position() const;

    /**
     * @return Size.
     */
    const glm::vec2& size() const;

    /**
     * @return Left limit.
     */
    float left() const;

    /**
     * @return Right limit.
     */
    float right() const;

    /**
     * @return Top limit.
     */
    float top() const;

    /**
     * @return Bottom limit.
     */
    float bottom() const;

    void draw(float dt, float interp, Renderer& renderer) override;

private:

    /** @brief Position. */
    glm::vec2 mPosition;

    /** @brief Size. */
    glm::vec2 mSize;
};

////////////////////////////////////////////////////////////

} // namespace pong
