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

#include <glm/glm.hpp>

namespace pong {

/**
 * @class Renderer
 * @brief Defines a pure abstract interface for a rendering system.
 *
 * @details This class provides a common API for all rendering backends (e.g., an OpenGL implementation). It defines the
 * contract for initializing, shutting down, and performing frame-by-frame rendering operations.
 *
 * This class is non-copyable and non-movable as it represents a unique system resource.
 * The intended lifecycle is:
 *     construction -> `init()` -> frame loop (`beginFrame`/`endFrame`) -> `quit()` -> destruction.
 */
class Renderer
{
protected:

    /**
     * @brief Protected default constructor to allow inheritance.
     */
    Renderer() = default;

public:

    Renderer(const Renderer&) = delete;

    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;

    Renderer& operator=(Renderer&&) = delete;

    /**
     * @brief Virtual destructor to ensure proper cleanup in derived classes.
     */
    virtual ~Renderer() = default;

    /**
     * @brief Prepares the renderer for a new frame.
     *
     * This should be called once at the beginning of each frame's rendering phase. It typically clears the screen to a
     * default color.
     */
    virtual void beginFrame() = 0;

    /**
     * @brief Finalizes the frame and presents it to the screen.
     *
     * This should be called once at the end of each frame's rendering phase. It typically swaps the front and
     * back buffers to display the rendered content.
     */
    virtual void endFrame() = 0;

    /**
     * @brief Adds a solid white quadrilateral to the render queue for the current frame.
     * @param position The center position of the quad, in game units.
     * @param size The width and height of the quad, in game units.
     */
    virtual void queueQuad(const glm::vec2& position, const glm::vec2& size) = 0;

    /**
     * @brief Adds a solid-colored quadrilateral to the render queue for the current frame.
     * @param position The center position of the quad, in game units.
     * @param size The width and height of the quad, in game units.
     * @param color The RGBA color of the quad.
     */
    virtual void queueQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) = 0;
};

} // namespace pong
