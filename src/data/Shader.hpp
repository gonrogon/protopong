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

#include <string_view>
#include <array>

namespace pong::data {

/**
 * @brief Vertex data for a 2D quad made of two triangles.
 * Covers the screen area from -0.5 to 0.5.
 */
inline constexpr std::array QuadVertices =
{
    -0.5f,  0.5f,
    -0.5f, -0.5f,
     0.5f,  0.5f,
     0.5f,  0.5f,
    -0.5f, -0.5f,
     0.5f, -0.5f
};

/**
 * @brief Vertex shader source code for OpenGL 3.3+ Core profile.
 */
inline constexpr std::string_view GL3VS = R"(
#version 330 core

layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;

uniform mat4 transform;

out vec4 vss_color;

void main()
{
    gl_Position = vec4(v_position, 0.0, 1.0) * transform;
    vss_color   = v_color;
}
)";

/**
 * @brief Fragment shader source code for OpenGL 3.3+ Core profile.
 */
inline constexpr std::string_view GL3FS = R"(
#version 330 core

in  vec4 vss_color;
out vec4 fragColor;

void main()
{
    fragColor = vss_color;
}
)";

} // namespace pong::data
