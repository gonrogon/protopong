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

#include "RendererGL3.hpp"
#include "data/Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <iostream>

namespace pong {
namespace      {

/**
 * @brief Creates and initializes a vertex buffer object (VBO) for a standard quad.
 *
 * Generates an OpenGL buffer, binds it, and uploads static vertex data suitable for rendering a quad composed of two
 * triangles.
 * @param id A pointer that will receive the handle of the newly created VBO.
 * @return True on success, false otherwise.
 */
bool createQuadVBO(GLuint* id);

/**
 * @brief Creates and configures a vertex array object (VAO) for a quad.
 *
 * Generates a VAO and configures its vertex attribute pointers to match the layout of the data in the provided VBO.
 * This effectively links the vertex data to the shader pipeline's input.
 * @param id A pointer that will receive the handle of the newly created VAO.
 * @param vbo The handle of an existing VBO that contains the vertex data for the quad.
 * @return True on success, false otherwise.
 */
bool createQuadVAO(GLuint* id, GLuint vbo);

/**
 * @brief Compiles and links a complete GLSL shader program from predefined sources.
 *
 * This function handles the entire shader pipeline: creating shader objects, compiling vertex and fragment shaders from
 * source, attaching them to a program object, and linking the final program. It performs error checking at each stage.
 * @param id A pointer that will receive the handle of the newly created program.
 * @return True on success, false otherwise.
 */
bool createProgram(GLuint* id);

/**
 * @brief Prints the information log for a given GLSL shader object.
 * If a shader failed to compile, this function retrieves the error log from OpenGL and prints it to the standard error
 * stream for debugging.
 * @param id The OpenGL handle of the shader object to query.
 * @param name A descriptive name for the shader (e.g., "vertex") to use in the log output.
 */
void printShaderLog(GLuint id, const char* name);

/**
 * @brief Prints the information log for a given GLSL program object.
 *
 * If a program failed to link, this function retrieves the error log from OpenGL and prints it to the standard error
 * stream for debugging.
 * @param id The OpenGL handle of the program object to query.
 */
void printProgramLog(GLuint id);

/**
 * @brief Clears the OpenGL error stack completely.
 *
 * Repeatedly calls `glGetError()` in a loop until it returns `GL_NO_ERROR`. This is useful to call before a specific
 * block of OpenGL operations to ensure that any subsequent error checks are not affected by pre-existing errors.
 */
void clearErrors();

/**
 * @brief Checks if any OpenGL errors have occurred.
 *
 * Drains the entire OpenGL error queue by calling `glGetError()` in a loop. If any call returns an error code, the
 * function reports an error.
 * @return True if at least one OpenGL error was detected, false otherwise.
 */
bool checkErrors();

} // namespace

struct RendererGL3::QuadVertex
{
    float px; //!< X-coordinate of the position.
    float py; //!< Y-coordinate of the position.
    float cr; //!< Red component of the color.
    float cg; //!< Green component of the color.
    float cb; //!< Blue component of the color.
    float ca; //!< Alpha component fo the color.
};

std::unique_ptr<Renderer> RendererGL3::create(const int width, const int height)
{
    const int w = width  <= 0 ? 640 :width;
    const int h = height <= 0 ? 480 :height;

    auto renderer = std::unique_ptr<RendererGL3>(new RendererGL3{});
    if (renderer->init(w, h)) {
        return renderer;
    }

    return nullptr;
}


bool RendererGL3::init(const int screenWidth, const int screenHeight)
{
    mScreenWidth  = screenWidth;
    mScreenHeight = screenHeight;
    // Create the buffers and shader programs.
    if (!createQuadVBO(mQuadVBO.idPtr()) || !createQuadVAO(mQuadVAO.idPtr(), mQuadVBO) || !createProgram(mProgram.idPtr()))
    {
        return false;
    }
    // Get the location of the uniform in the shader program.
    mLocTransform = glGetUniformLocation(mProgram, "transform");
    mLocSize      = glGetUniformLocation(mProgram, "size");
    mLocPosition  = glGetUniformLocation(mProgram, "position");
    mLocColor     = glGetUniformLocation(mProgram, "color");
    // Set the viewport to fill the screen.
    glViewport(0, 0, mScreenWidth, mScreenHeight);
    // Calculate the screen aspect (width / height).
    const float aspect = screenHeight == 0 ? 1.0f : static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    // Calculate the projection matrix.
    mProjection = glm::ortho(-100.0f * aspect, 100.0f * aspect, -100.0f, 100.0f, -1.0f, 1.0f);

    return true;
}

void RendererGL3::beginFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererGL3::endFrame()
{
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glBindVertexArray(mQuadVAO);
    glUseProgram     (mProgram);
    // The transformation matrix is the same for all quads.
    glUniformMatrix4fv(mLocTransform, 1, GL_FALSE, glm::value_ptr(mProjection));

    const std::size_t vertices = mQuads.size();
          std::size_t offset   = 0;
    // Upload and draw each batch of vertices.
    while (offset < vertices)
    {
        // Calculate the number of vertices to upload in this batch.
        const std::size_t count = std::min(vertices - offset, VerticesPerBatch);
        // Update the buffer and draw.
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(QuadVertex), mQuads.data() + offset);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
        // Move the offset.
        offset += count;
    }
    // Unbind the buffer to avoid unwanted access.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Clear the list of quads to be ready for the next iteration.
    mQuads.clear();
}

void RendererGL3::queueQuad(const glm::vec2& position, const glm::vec2& size)
{
    queueQuad(position, size, glm::vec4(1.0f));
}

void RendererGL3::queueQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    for (std::size_t i = 0; i < data::QuadVertices.size(); i += 2)
    {
        mQuads.emplace_back(
            data::QuadVertices[i]     * size.x + position.x,
            data::QuadVertices[i + 1] * size.y + position.y,
            color.r,
            color.g,
            color.b,
            color.a
        );
    }
}

namespace {

bool createQuadVBO(GLuint* id)
{
    clearErrors();
    // Generate the buffer and bind it.
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, *id);
    // Update the data.
    glBufferData(GL_ARRAY_BUFFER, RendererGL3::VerticesPerBatch * sizeof(RendererGL3::QuadVertex), nullptr, GL_DYNAMIC_DRAW);
    // Unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return !checkErrors();
}

bool createQuadVAO(GLuint* id, const GLuint vbo)
{
    clearErrors();
    // Generate the buffer and bind it.
    glGenVertexArrays(1, id);
    glBindVertexArray(*id);
    // Bind the quad VBO.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pointers.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    // Unbind.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return !checkErrors();
}

bool createProgram(GLuint* id)
{
    clearErrors();
    // Sources.
    const GLchar* vsSrc[1] = {data::GL3VS.data()};
    const GLchar* fsSrc[1] = {data::GL3FS.data()};
    // Create the vertex and frame shader programs.
    const GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);
    const auto vsLen = static_cast<GLint>(data::GL3VS.length());
    const auto fsLen = static_cast<GLint>(data::GL3FS.length());
    GLint status1 = 0;
    GLint status2 = 0;
    // Sources.
    glShaderSource(vsId, 1, vsSrc, &vsLen);
    glShaderSource(fsId, 1, fsSrc, &fsLen);
    // Compile.
    glCompileShader(vsId);
    glCompileShader(fsId);
    glGetShaderiv  (vsId, GL_COMPILE_STATUS, &status1);
    glGetShaderiv  (fsId, GL_COMPILE_STATUS, &status2);
    // Check compilation result.
    if (status1 == static_cast<GLint>(GL_FALSE) || status2 == static_cast<GLint>(GL_FALSE))
    {
        printShaderLog(vsId, "Vertex");
        printShaderLog(fsId, "Fragment");
        glDeleteShader(vsId);
        glDeleteShader(fsId);

        return false;
    }
    // Create program.
    const GLuint programId = glCreateProgram();
    // Attach the previous shader to the shader program.
    glAttachShader(programId, vsId);
    glAttachShader(programId, fsId);
    // Link.
    glLinkProgram (programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &status1);
    // Check linking result.
    if (status1 == static_cast<GLint>(GL_FALSE))
    {
        printProgramLog(programId);
        // Clear resources.
        glDeleteShader (vsId);
        glDeleteShader (fsId);
        glDeleteProgram(programId);
        // Error when linking the program.
        return false;
    }
    // Save.
    *id = programId;
    // Clear.
    glDeleteShader(vsId);
    glDeleteShader(fsId);

    return !checkErrors();
}

void printShaderLog(const GLuint id, const char* name)
{
    GLint logLen;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);

    if (logLen > 0)
    {
        std::vector<GLchar> log(static_cast<unsigned int>(logLen));

        glGetShaderInfoLog(id, logLen, &logLen, log.data());

        std::cerr << name << ":" << std::endl;
        std::cerr << log.data()  << std::endl;
    }
}

void printProgramLog(const GLuint id)
{
    GLint logLen;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLen);

    if (logLen > 0)
    {
        std::vector<GLchar> log(static_cast<unsigned int>(logLen));

        glGetProgramInfoLog(id, logLen, &logLen, log.data());

        std::cerr << "Program:" << std::endl;
        std::cerr << log.data() << std::endl;
    }
}

void clearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool checkErrors()
{
    bool   error  = false;
    while((error |= (glGetError() != GL_NO_ERROR))) {}

    return error;
}

} // namespace
} // namespace pong
