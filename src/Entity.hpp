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
/// @file   src/Entity.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Require.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an abstract class for entities.
///
/// All scene elements are entities.
////////////////////////////////////////////////////////////
class Entity
{
public:

    /**
     * @brief Define an enumeration with the types of entity.
     */
    enum class Type
    {
        Paddle, //!< Paddle.
        Ball,   //!< Ball.
        Table,  //!< Table.
        Label   //!< Text label.
    };

protected:

    /**
     * @brief Constructor.
     *
     * @param type Type of entity.
     */
    explicit Entity(Type type) : mType(type) {}

public:

    /**
     * @brief Constructor (copy).
     */
    Entity(const Entity&) = delete;

    /**
     * @brief Destructor.
     */
    virtual ~Entity() = default;

    /**
     * @return Type of entity.
     */
    Type type() const { return mType; }

    /**
     * @return Read/write reference to the scene.
     */
    Scene& scene() const { return *mScene; }

    /**
     * @brief Set the scene that contains this entity.
     *
     * This method is called by the scene when an entity is inserted.
     *
     * @param scene Scene to set.
     */
    void setScene(Scene& scene) { mScene = &scene; }

    /**
     * @brief Handle an event.
     *
     * @param event Event to handle.
     */
    virtual void handle(const Event& event) {}

    /**
     * @brief Update the entity.
     *
     * @param dt Time elapsed since the last update.
     */
    virtual void update(float dt) {}

    /**
     * @brief Draw the entity.
     *
     * @param dt       Time elapsed since the last draw.
     * @param interp   Interpolation value.
     * @param renderer Renderer.
     */
    virtual void draw(float dt, float interp, Renderer& renderer) {}

private:

    /** @brief Type of entity. */
    Type mType;

    /** @brief Scene. */
    Scene* mScene = nullptr;
};

////////////////////////////////////////////////////////////

} // namespace pong
