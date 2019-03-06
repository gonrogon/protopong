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
/// @file   src/Renderer.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Require.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define a renderer.
////////////////////////////////////////////////////////////
class Renderer
{
protected:

    /**
     * @brief Constructor.
     */
    Renderer() = default;

public:

    /**
     * @brief Destructor.
     */
    virtual ~Renderer() = default;

    /**
     * @brief Constructor (copy).
     */
    Renderer(const Renderer&) = delete;

    /**
     * @brief Assignment operator (copy).
     */
    Renderer& operator=(const Renderer&) = delete;

    /**
     * @brief Initialize the renderer.
     *
     * @param screenWidth Screen width.
     * @param screenHeight Screen height.
     */
    virtual bool init(unsigned int screenWidth, unsigned int screenHeight) = 0;

    /**
     * @brief Finalize the renderer.
     */
    virtual void quit() = 0;

    /**
     * @brief Initiates the rendering.
     */
    virtual void begin() = 0;

    /**
     * @brief Finalizes the rendering.
     */
    virtual void end() = 0;

    /**
     * @brief Adds a white quad to the render queue.
     *
     * @param size     Size.
     * @param position Position.
     */
    virtual void queueQuad(const glm::vec2& position, const glm::vec2& size) = 0;

    /**
     * @brief Adds a quad to the render queue.
     *
     * @param size     Size.
     * @param position Position.
     * @param color    Color.
     */
    virtual void queueQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) = 0;
};

////////////////////////////////////////////////////////////

} // namespace pong
