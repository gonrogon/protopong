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

namespace pong {

class Renderer;
class Scene;
class Event;

/**
 * @class Entity
 * @brief An abstract base class for all game objects.
 *
 * @details Every object in a scene, from players to UI elements, should derive from this class. It provides a common
 * interface for event handling, state updates, and rendering.
 */
class Entity
{
public:

    /**
     * @brief Define an enumeration with the specific type of entity.
     */
    enum class Type
    {
        Paddle, //!< A player's paddle.
        Ball,   //!< The game ball.
        Table,  //!< The game area.
        Label   //!< A text label for UI elements.
    };

protected:

    /**
     * @brief Constructs.
     * @param type The specific type of the derived entity.
     */
    explicit Entity(const Type type) noexcept : mType(type) {}

public:

    Entity(const Entity&) = delete;

    Entity(Entity&&) = delete;

    Entity& operator=(const Entity&) = delete;

    Entity& operator=(Entity&&) = delete;

    virtual ~Entity() = default;

    /**
     * @brief Gets the type of the entity.
     * @return Type of entity.
     */
    [[nodiscard]] Type type() const noexcept { return mType; }

    /**
     * @brief Gets a pointer to the scene that contains this entity.
     * @warning This method can return nullptr if the entity is not currently attached to a scene.
     * @return A pointer to the scene, or null if the entity is not in a scene.
     */
    [[nodiscard]] Scene* scene() const noexcept { return mScene; }

    /**
     * @brief Sets the scene that contains this entity.
     * @details This method is typically called by the `Scene` class itself when an entity is added to or removed from
     * it.
     * @param scene A pointer to the scene, or null to detach the entity.
     */
    void setScene(Scene* scene) noexcept { mScene = scene; }

    /**
     * @brief Handles a game event.
     *
     * Derived classes should override this method to react to relevant events.
     * @param event The event to be processed.
     */
    virtual void handle(const Event& event) {}

    /**
     * @brief Updates the entity's internal state based on elapsed time.
     *
     * This is intended for physics or logic updates. Derived classes should override this method to implement
     * their behavior.
     * @param dt The time elapsed since the last update frame (delta time).
     */
    virtual void update(const TimeDuration dt) {}

    /**
     * @brief Draws the entity to the screen.
     *
     * Derived classes must override this method to render themselves.
     * @param renderer The renderer object to use for drawing operations.
     * @param interp A value between 0.0 and 1.0 for physics state interpolation, used to achieve smooth rendering.
     */
    virtual void draw(Renderer& renderer, const float interp) {}

private:

    /** @brief The specific type of this entity. */
    Type mType;

    /** @brief A pointer to the scene that owns this entity or null is the entity is not attached to a scene. */
    Scene* mScene = nullptr;
};

} // namespace pong
