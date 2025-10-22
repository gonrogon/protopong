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

#include "Time.hpp"
#include <memory>
#include <vector>

namespace pong {

class Renderer;
class Entity;
class Game;

/**
 * @brief Manages a collection of game entities and their lifecycle.
 *
 * The Scene is the primary container for all active game objects (`Entity`).
 * It is responsible for:
 * - **Ownership:** Exclusively owns all entities using `std::unique_ptr`, ensuring automatic memory management and
 *   preventing leaks.
 * - **Lifecycle:** Driving the main game loop for its entities by calling their `update()` and `draw()` methods each
 *   frame.
 * - **Access:** Providing a way to access entities for interaction.
 */
class Scene
{
public:

    /**
     * @brief Constructs a new scene.
     * @param game A reference to the main game object, which owns this scene.
     */
    explicit Scene(Game& game);

    Scene(const Scene&) = delete;

    Scene(const Scene&&) = delete;

    Scene& operator=(const Scene&) = delete;

    Scene& operator=(Scene&&) = delete;

    /**
     * @brief Destructor.
     * Automatically destroys all entities contained within the scene.
     */
    ~Scene();

    /**
     * @brief Gets a reference to the parent game object.
     * @return A reference to the game.
     */
    [[nodiscard]] Game& game() const noexcept { return mGame; }

    /**
     * @brief Gets a reference to an entity by its index.
     * @warning This method does not check if the entity exists.
     * @note The method is `const` because it does not modify the Scene's container, but the returned entity itself can
     * be modified.
     * @param i The zero-based index of the entity to retrieve.
     * @return A reference to the entity at the specified index.
     */
    [[nodiscard]] Entity& at(int i) const;

    /**
     * @brief Adds a new entity to the scene, taking ownership of it.
     *
     * The provided `std::unique_ptr` is moved into the scene's internal storage. After this call, the original pointer
     * will be null, and the scene becomes solely responsible for the entity's lifetime.
     * @param entity Unique pointer to the entity being added.
     * @return The index assigned to the newly added entity.
     */
    int append(std::unique_ptr<Entity> entity);

    /**
     * @brief Emplaces a new entity into the scene, constructing it in-place.
     * @details This is a convenience factory method that constructs a new entity of type `T` in-place and adds it to
     * the scene. It simplifies object creation and addition into a single, safe operation, mirroring the behavior of
     * container methods like `emplace_back`.
     * @tparam T The concrete `Entity` type to create (e.g., `Label`, `Matrix`).
     * @tparam Args The types of the arguments to forward to the constructor.
     * @param args The arguments for the constructor.
     * @return A non-owning raw pointer to the newly created entity on success, null on failure.
     */
    template<typename T, typename... Args> requires std::derived_from<T, Entity>
    T* emplace(Args&&... args)
    {
        const int index = append(std::make_unique<T>(std::forward<Args>(args)...));
        if (index < 0)
        {
            return nullptr;
        }

        return &static_cast<T&>(at(index));
    }

    /**
     * @brief Removes all entities from the scene.
     */
    void clear();

    /**
     * @brief Updates the state of all entities in the scene.
     * @param dt The time elapsed since the last update frame (delta time).
     */
    void update(TimeDuration dt);

    /**
     * @brief Draws all entities in the scene.
     * @param renderer The renderer object to use for drawing operations.
     * @param interp The interpolation value (between 0.0 and 1.0) for smooth rendering.
     */
    void draw(Renderer& renderer, float interp);

private:

    /** @brief Parent Game object. */
    Game& mGame;

    /** @brief Vector with all entities in the scene. */
    std::vector<std::unique_ptr<Entity>> mEntities;
};

} // namespace pong
