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
/// @file   src/RendererGL3.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "RendererGL3.hpp"
#include "data/Shader.hpp"
//#include <GL/glew.h>
#include <glad/glad.h>
#include <iostream>
#include <cstring>

////////////////////////////////////////////////////////////

namespace pong {
namespace      {

////////////////////////////////////////////////////////////

/**
 * @brief Creates the quad VBO.
 *
 * @param id VBO id.
 *
 * @return True on success; otherwise, false.
 */
bool createQuadVBO(GLuint& id);

/**
 * @brief Creates the quad VAO.
 *
 * @param id  VAO id.
 * @param vbo VBO id.
 *
 * @return True on success; otherwise, false.
 */
bool createQuadVAO(GLuint& id, GLuint vbo);

/**
 * @brief Creates the shader program.
 *
 * @param id Program id.
 *
 * @return True on success; otherwise, false.
 */
bool createProgram(GLuint& id);

/**
 * @brief Prints the error log.
 *
 * @param id   Shader id.
 * @param name Shader name to print.
 */
void printShaderLog(GLuint id, const char* name);

/**
 * @brief Prints the error log.
 *
 * @param id   Program id.
 * @param name Program name to print.
 */
void printProgramLog(GLuint id, const char* name);

/**
 * @brief Clear OpenGL error stack.
 */
void clearErrors();

/**
 * @brief Check if there is an OpenGL error.
 *
 * @return True if there is an error; otherwise, false.
 */
bool checkErrors();

////////////////////////////////////////////////////////////

} // namespace

////////////////////////////////////////////////////////////

struct RendererGL3::Quad
{
    glm::vec2 mSize;     //!< Size.
    glm::vec2 mPosition; //!< Position.
    glm::vec4 mColor;    //!< Color.
};

////////////////////////////////////////////////////////////

RendererGL3::RendererGL3() = default;

////////////////////////////////////////////////////////////

bool RendererGL3::init(unsigned int screenWidth, unsigned int screenHeight)
{
    mScreenWidth  = screenWidth;
    mScreenHeight = screenHeight;
    // Create the buffers and shader programs.
    if (!createQuadVBO(mQuadVBO) || !createQuadVAO(mQuadVAO, mQuadVBO) || !createProgram(mProgram))
    {
        return false;
    }
    // Set the viewport to fill the screen.
    glViewport(0, 0, static_cast<GLsizei>(mScreenWidth), static_cast<GLsizei>(mScreenHeight));
    // Calculate the screen aspect (width / height).
    float aspect = screenHeight == 0 ? 1.0f : static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    // Calculate the projection matrix.
    mProjection = glm::ortho(-100.0f * aspect, 100.0f * aspect, -100.0f, 100.0f, -1.0f, 1.0f);

    return true;
}

////////////////////////////////////////////////////////////

void RendererGL3::quit()
{
    clear();
}

////////////////////////////////////////////////////////////

void RendererGL3::clear()
{
    glDeleteVertexArrays(1, &mQuadVAO); mQuadVAO = 0;
    glDeleteBuffers     (1, &mQuadVBO); mQuadVBO = 0;
    glDeleteProgram         (mProgram); mProgram = 0;
}

////////////////////////////////////////////////////////////

void RendererGL3::begin()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

////////////////////////////////////////////////////////////

void RendererGL3::end()
{
    glUseProgram     (mProgram);
    glBindVertexArray(mQuadVAO);
    // Locate the uniforms.
    GLint locT = glGetUniformLocation(mProgram, "transform");
    GLint locS = glGetUniformLocation(mProgram, "size");
    GLint locP = glGetUniformLocation(mProgram, "position");
    GLint locC = glGetUniformLocation(mProgram, "color");
    // The transformation matrix is the same for all quads.
    glUniformMatrix4fv(locT, 1, GL_FALSE, glm::value_ptr(mProjection));
    // Draw each quad.
    for (Quad& quad : mQuads)
    {
        glUniform2fv(locS, 1, glm::value_ptr(quad.mSize));
        glUniform2fv(locP, 1, glm::value_ptr(quad.mPosition));
        glUniform4fv(locC, 1, glm::value_ptr(quad.mColor));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    // Clear the list of quads to be ready for the next iteration.
    mQuads.clear();
}

////////////////////////////////////////////////////////////

void RendererGL3::queueQuad(const glm::vec2& position, const glm::vec2& size)
{
    mQuads.emplace_back(Quad{size, position, {1.0f, 1.0f, 1.0f, 1.0f}});
}

////////////////////////////////////////////////////////////

void RendererGL3::queueQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    mQuads.emplace_back(Quad{size, position, color});
}

////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////

bool createQuadVBO(GLuint& id)
{
    clearErrors();
    // Generate the buffer and bind it.
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    // Update the data.
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), data::Shader::Quad, GL_STATIC_DRAW);
    // Unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Done.
    return !checkErrors();
}

////////////////////////////////////////////////////////////

bool createQuadVAO(GLuint& id, const GLuint vbo)
{
    clearErrors();
    // Generate the buffer and bind it.
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    // Bind the quad VBO.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pointers.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    // Unbind.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Done.
    return !checkErrors();
}

////////////////////////////////////////////////////////////

bool createProgram(GLuint& id)
{
    clearErrors();
    // Sources.
    const GLchar* vssrc[1] = {data::Shader::GL3_vs};
    const GLchar* fssrc[1] = {data::Shader::GL3_fs};
    // Create the vertex and frame shader programs.
    const GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
    auto  vslen = static_cast<GLint>(std::strlen(data::Shader::GL3_vs));
    auto  fslen = static_cast<GLint>(std::strlen(data::Shader::GL3_fs));
    GLint status1 = 0;
    GLint status2 = 0;
    // Sources.
    glShaderSource(vsid, 1, vssrc, &vslen);
    glShaderSource(fsid, 1, fssrc, &fslen);
    // Compile.
    glCompileShader(vsid);
    glCompileShader(fsid);
    glGetShaderiv  (vsid, GL_COMPILE_STATUS, &status1);
    glGetShaderiv  (fsid, GL_COMPILE_STATUS, &status2);
    // Check compilation result.
    if (status1 == static_cast<GLint>(GL_FALSE) || status2 == static_cast<GLint>(GL_FALSE))
    {
        printShaderLog(vsid, "Vertex");
        printShaderLog(fsid, "Fragment");

        glDeleteShader(vsid);
        glDeleteShader(fsid);

        return false;
    }
    // Create program.
    const GLuint prid = glCreateProgram();
    // Attach the previous shader to the shader program.
    glAttachShader(prid, vsid);
    glAttachShader(prid, fsid);
    // Link.
    glLinkProgram (prid);
    glGetProgramiv(prid, GL_LINK_STATUS, &status1);
    // Check linking result.
    if (status1 == static_cast<GLint>(GL_FALSE))
    {
        printProgramLog(prid, "Program");
        // Clear resources.
        glDeleteShader (vsid);
        glDeleteShader (fsid);
        glDeleteProgram(prid);
        // Error when linking the program.
        return false;
    }
    // Save.
    id = prid;
    // Clear.
    glDeleteShader(vsid);
    glDeleteShader(fsid);
    // Done.
    return !checkErrors();
}

////////////////////////////////////////////////////////////

void printShaderLog(const GLuint id, const char* name)
{
    GLint loglen;

    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglen);

    if (loglen > 0)
    {
        std::vector<GLchar> log(static_cast<unsigned int>(loglen));

        glGetShaderInfoLog(id, loglen, &loglen, log.data());

        std::cerr << name << ":" << std::endl;
        std::cerr << log.data()  << std::endl;
    }
}

////////////////////////////////////////////////////////////

void printProgramLog(const GLuint id, const char* name)
{
    GLint loglen;

    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &loglen);

    if (loglen > 0)
    {
        std::vector<GLchar> log(static_cast<unsigned int>(loglen));

        glGetProgramInfoLog(id, loglen, &loglen, log.data());

        std::cerr << name << ":" << std::endl;
        std::cerr << log.data()  << std::endl;
    }
}

////////////////////////////////////////////////////////////

void clearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

////////////////////////////////////////////////////////////

bool checkErrors()
{
    bool   error  = false;
    while((error |= (glGetError() != GL_NO_ERROR)));

    return error;
}

////////////////////////////////////////////////////////////

} // namespace
} // namespace pong
