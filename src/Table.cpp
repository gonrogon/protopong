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
/// @file   src/Table.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Table.hpp"

#include "RendererGL3.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

Table::Table(const glm::vec2& position, const glm::vec2& size)
    :
    Entity   (Type::Table),
    mPosition(position),
    mSize    (size)
{}

////////////////////////////////////////////////////////////

const glm::vec2& Table::position() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////

const glm::vec2& Table::size() const
{
    return mSize;
}

////////////////////////////////////////////////////////////

float Table::left() const
{
    return mPosition.x - mSize.x * 0.5f;
}

////////////////////////////////////////////////////////////

float Table::right() const
{
    return mPosition.x + mSize.x * 0.5f;
}

////////////////////////////////////////////////////////////

float Table::top() const
{
    return mPosition.y + mSize.y * 0.5f;
}

////////////////////////////////////////////////////////////

float Table::bottom() const
{
    return mPosition.y - mSize.y * 0.5f;
}

////////////////////////////////////////////////////////////

void Table::draw(const float dt, const float interp, Renderer& renderer)
{
    renderer.queueQuad({mPosition.x, top()},       {mSize.x, 0.50f}, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer.queueQuad({mPosition.x, bottom()},    {mSize.x, 0.50f}, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer.queueQuad({left(),      mPosition.y}, {0.50f, mSize.y}, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer.queueQuad({right(),     mPosition.y}, {0.50f, mSize.y}, {1.0f, 1.0f, 1.0f, 1.0f});
    renderer.queueQuad(mPosition,                  {0.50f, mSize.y}, {1.0f, 1.0f, 1.0f, 1.0f});
}

////////////////////////////////////////////////////////////

} // namespace pong
