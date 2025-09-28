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

#include "Renderer.hpp"
#include "RendererGL3Util.hpp"
#include <memory>
#include <vector>

namespace pong {

/**
 * @brief A concrete implementation of the Renderer interface using OpenGL 3.3.
 *
 * This class handles all rendering for the game using a batch-based system for drawing quadrilaterals. It is designed
 * with modern C++ principles in mind:
 *
 * - **Factory Creation:** It must be instantiated via the static `create()` method.
 * - **RAII:** All OpenGL resources (VBO, VAO, shaders) are managed automatically by RAII handles, guaranteeing no
 *   resource leaks.
 */
class RendererGL3 final : public Renderer
{
public:

    static constexpr std::size_t QuadsPerBatch = 1024;

    static constexpr std::size_t VerticesPerBatch = QuadsPerBatch * 6;

    /**
     * @brief A private struct representing a single quadrilateral to be rendered in a batch.
     */
    struct QuadVertex;

    /**
     * @brief Factory method to create and initialize a `RendererGL3` instance.
     *
     * This is the sole entry point for creating a renderer. It handles all necessary OpenGL setup and resource
     * allocation.
     * @param width The desired width of the rendering window, in pixels.
     * @param height The desired height of the rendering window, in pixels.
     * @return A unique pointer holding the new instance if initialization is successful, or null if it fails.
     */
    static std::unique_ptr<Renderer> create(int width, int height);

    /**
     * @brief Default destructor.
     *
     * Automatically releases all managed OpenGL resources thanks to the RAII handles for VBO, VAO, and Program. No
     * explicit cleanup is needed.
     */
    ~RendererGL3() override = default;

private:

    /**
     * @brief Private default constructor to enforce creation via the factory method.
     */
    RendererGL3() = default;

    /**
     * @brief Performs the actual initialization of OpenGL resources.
     *
     * This method is called internally by the `create()` factory. It sets up buffers, compiles shaders, and caches
     * uniform locations.
     * @param screenWidth The width of the screen.
     * @param screenHeight The height of the screen.
     * @return True on success, false on failure.
     */
    bool init(int screenWidth, int screenHeight);

public:

    void beginFrame() override;

    void endFrame() override;

    void queueQuad(const glm::vec2& position, const glm::vec2& size) override;

    void queueQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) override;

private:

    /** @brief The width of the rendering surface, in pixels. */
    int mScreenWidth = 0;

    /** @brief The height of the rendering surface, in pixels. */
    int mScreenHeight = 0;

    /** @brief RAII handle for the quad Vertex Buffer Object. Stores vertex data. */
    GL3VBOHandle mQuadVBO;

    /** @brief RAII handle for the quad Vertex Array Object. Stores vertex attribute state. */
    GL3VAOHandle mQuadVAO;

    /** @breif RAII handle for the GLSL shader program. */
    GL3ProgramHandle mProgram;

    /** @brief Cached location of the 'transform' uniform in the shader. */
    GLint mLocTransform = -1;

    /** @brief Cached location of the 'size' uniform in the shader. */
    GLint mLocSize = -1;

    /** @brief Cached location of the 'position' uniform in the shader. */
    GLint mLocPosition = -1;

    /** @brief Cached location of the 'color' uniform in the shader. */
    GLint mLocColor = -1;

    /** @brief The orthographic projection matrix used to map world space to screen space. */
    glm::mat4 mProjection = glm::mat4(1.0f);

    /** @brief A vector that batches all quads to be drawn in the current frame. */
    std::vector<QuadVertex> mQuads;
};

} // namespace pong
