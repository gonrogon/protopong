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
/// @file   src/RendererGL3.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Renderer.hpp"
#include <vector>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define a renderer.
////////////////////////////////////////////////////////////
class RendererGL3 : public Renderer
{
private:

    /**
     * @brief Define a quad.
     */
    struct Quad;

    /**
     * @brief Define a type for a list of quads.
     */
    typedef std::vector<Quad> Quads;

public:

    /**
     * @brief Constructor.
     */
    RendererGL3();

    bool init(unsigned int screenWidth, unsigned int screenHeight) override;

    void quit() override;

private:

    /**
     * @brief Releases the resources.
     */
    void clear();

public:

    void begin() override;

    void end() override;

    void queueQuad(const glm::vec2& position, const glm::vec2& size) override;

    void queueQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) override;

private:

    /** @brief Screen width. */
    unsigned int mScreenWidth = 0;

    /** @brief Screen height. */
    unsigned int mScreenHeight = 0;

    /** @brief Vertex buffer object for a quad. */
    unsigned int mQuadVBO = 0;

    /** @brief Vertex array object for a quad. */
    unsigned int mQuadVAO = 0;

    /** @brief Shader program. */
    unsigned int mProgram = 0;

    /** @brief Projection matrix. */
    glm::mat4 mProjection = glm::mat4(1.0f);

    /** @brief List of quad to draw. */
    Quads mQuads;
};

////////////////////////////////////////////////////////////

} // namespace pong
