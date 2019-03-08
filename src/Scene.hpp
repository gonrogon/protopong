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
/// @file   src/Scene.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Require.hpp"
#include <memory>
#include <vector>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define a scene.
///
/// A scene is a group of entities with a logical relationship.
////////////////////////////////////////////////////////////
class Scene
{
private:

    /** @brief Define a type for unique pointers to entities. */
    typedef std::unique_ptr<Entity> EntityPtr;

    /** @brief Define a type for vectors of entities. */
    typedef std::vector<EntityPtr> Entities;

public:

    /**
     * @brief Constructor.
     *
     * @param game Game.
     */
    explicit Scene(Game& game);

    /**
     * @brief Constructor (copy).
     */
    Scene(const Scene&) = delete;

    /**
     * @brief Destructor.
     */
    ~Scene();

    /**
     * @return Game.
     */
    Game& game() const { return mGame; }

    /**
     * @brief Gets an entity.
     *
     * @warning This method does not check if the entity exists.
     *
     * @param i Index.
     *
     * @return Read/write reference to the entity.
     */
    Entity& at(int i);

    /**
     * @brief Appends an entity.
     *
     * @param entity Entity to append.
     *
     * @return Index assigned to the entity.
     */
    int append(Entity* entity);

    /**
     * @brief Clears the scene.
     */
    void clear();

    /**
     * @brief Updates the scene.
     *
     * @param dt Time elapsed since the last update.
     */
    void update(float dt);

    /**
     * @brief Draws the scene.
     *
     * @param dt       Time elapsed since the last draw.
     * @param interp   Interpolation value.
     * @param renderer Renderer.
     */
    void draw(float dt, float interp, Renderer& renderer);

private:

    /** @brief Game. */
    Game& mGame;

    /** @brief List of entities. */
    Entities mEntities;
};

////////////////////////////////////////////////////////////

} // namespace pong
