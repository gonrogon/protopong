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
/// @file   src/data/Sound.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "data/Shader.hpp"

////////////////////////////////////////////////////////////

namespace pong {
namespace data {

////////////////////////////////////////////////////////////

const float Shader::Quad[] =
{
    -0.5f,  0.5f,
    -0.5f, -0.5f,
     0.5f,  0.5f,
     0.5f,  0.5f,
    -0.5f, -0.5f,
     0.5f, -0.5f
};

////////////////////////////////////////////////////////////

const char* Shader::GL2_vs = R"(
#version 120

in vec2 v_position;

uniform mat4 transform;
uniform vec2 size;
uniform vec2 position;
uniform vec4 color;

varying vec4 vss_color;

void main()
{
    gl_Position = vec4(v_position * size + position, 0.0, 1.0) * transform;
    vss_color   = color;
}
)";

////////////////////////////////////////////////////////////

const char* Shader::GL2_fs = R"(
#version 120

in vec4 vss_color;

void main()
{
    gl_FragColor = vss_color;
}
)";

////////////////////////////////////////////////////////////

const char* Shader::GL3_vs = R"(
#version 330 core

layout(location = 0) in vec2 v_position;

uniform mat4 transform;
uniform vec2 size;
uniform vec2 position;
uniform vec4 color;

out vec4 vss_color;

void main()
{
    gl_Position = vec4(v_position * size + position, 0.0, 1.0) * transform;
    vss_color   = color;
}
)";

////////////////////////////////////////////////////////////

const char* Shader::GL3_fs = R"(
#version 330 core

in  vec4 vss_color;
out vec4 fragColor;

void main()
{
    fragColor = vss_color;
}
)";

////////////////////////////////////////////////////////////

} // namespace data
} // namespace pong
