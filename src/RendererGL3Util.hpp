#pragma once

#include <glad/gl.h>
#include <utility>

namespace pong {

/**
 * @brief A generic RAII wrapper for an OpenGL object handle.
 *
 * This class manages the lifecycle of an OpenGL object (like a VBO, VAO, or Shader Program). It acquires the handle
 * upon creation and automatically releases it upon destruction.
 * It is non-copyable but movable, ensuring unique ownership of the GPU resource.
 *
 * @tparam Deleter A functor type whose operator() knows how to delete the OpenGL object.
 */
template<typename Deleter>
class GL3Handle
{
public:
    /**
     * @brief Constructs a null/empty handle.
     */
    GL3Handle() noexcept = default;

    /**
     * @brief Constructs a handle that takes ownership of an existing OpenGL object ID.
     * @param id OpenGL object ID.
     */
    explicit GL3Handle(const GLuint id) noexcept : m_id(id) {}

    /**
     * @brief Handles are non-copyable.
     */
    GL3Handle(const GL3Handle&) = delete;

    /**
     * @brief Move constructor. Transfers ownership of the resource from another handle.
     * @param other Handle to move.
     */
    GL3Handle(GL3Handle&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

    /**
     * @brief Handles are non-copy-assignable.
     */
    GL3Handle& operator=(const GL3Handle&) = delete;

    /**
     * @brief Move assignment operator. Releases the current resource and takes ownership from another.
     * @param other Handle to move.
     * @return Reference to itself.
     */
    GL3Handle& operator=(GL3Handle&& other) noexcept
    {
        if (this != &other)
        {
            if (m_id != 0)
            {
                Deleter{}(m_id);
            }
            m_id = std::exchange(other.m_id, 0);
        }

        return *this;
    }

    /**
     * @brief Destroys the handle and releases the underlying OpenGL resource.
     */
    ~GL3Handle()
    {
        if (m_id != 0)
        {
            Deleter{}(m_id);
        }
    }

    /**
     * @brief Implicit conversion to the underlying GLuint handle for use in gl* calls.
     */
    operator GLuint() const noexcept { return m_id; }

    /**
     * @brief Gets a pointer to the underlying ID.
     *
     * This is needed for API calls that write the ID into a pointer, such as `glGenBuffers`.
     * @warning Using this pointer after the handle has been moved from will result in undefined behavior.
     *
     * @return A pointer to the internal OpenGL object ID.
     */
    GLuint* idPtr() noexcept { return &m_id; }

private:

    /** @brief OpenGL object ID. */
    GLuint m_id = 0;
};

//==================//
// Deleter Functors //
//==================//

/** @brief Deleter for an OpenGL Vertex Buffer Object (VBO). */
struct GL3VBODeleter { void operator()(const GLuint id) const { glDeleteBuffers(1, &id); } };

/** @brief Deleter for an OpenGL Vertex Array Object (VAO). */
struct GL3VAODeleter { void operator()(const GLuint id) const { glDeleteVertexArrays(1, &id); } };

/** @brief  Deleter for an OpenGL shader program. */
struct GL3ProgramDeleter { void operator()(const GLuint id) const { glDeleteProgram(id); } };

//==============================//
// Type Aliases for Convenience //
//==============================//

/** @brief A RAII-managed handle for a VBO. */
using GL3VBOHandle = GL3Handle<GL3VBODeleter>;

/** @brief A RAII-managed handle for a VAO. */
using GL3VAOHandle = GL3Handle<GL3VAODeleter>;

/** @brief A RAII-managed handle for a shader program. */
using GL3ProgramHandle = GL3Handle<GL3ProgramDeleter>;

} // namespace pong