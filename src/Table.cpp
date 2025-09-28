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

#include "Table.hpp"
#include "Renderer.hpp"

namespace pong {
namespace {

/** @brief Width of the lines. */
constexpr float LineWidth = 0.5f;

/** @brief Color of the lines. */
constexpr glm::vec4 LineColor = {1.0f, 1.0f, 1.0f, 1.0f};

}

Table::Table(const glm::vec2& position, const glm::vec2& size) noexcept
    :
    Entity(Type::Table), mPosition(position), mSize(size)
{}

void Table::draw(Renderer& renderer, const float interp)
{
    renderer.queueQuad({mPosition.x, top()},       {mSize.x, LineWidth}, LineColor);
    renderer.queueQuad({mPosition.x, bottom()},    {mSize.x, LineWidth}, LineColor);
    renderer.queueQuad({left(),      mPosition.y}, {LineWidth, mSize.y}, LineColor);
    renderer.queueQuad({right(),     mPosition.y}, {LineWidth, mSize.y}, LineColor);
    renderer.queueQuad(mPosition,                  {LineWidth, mSize.y}, LineColor);
}

} // namespace pong
