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

#include "Scene.hpp"
#include "Entity.hpp"

namespace pong {

Scene::Scene(Game& game) : mGame(game) {}

Scene::~Scene() = default;

Entity& Scene::at(const int i) const
{
    return *mEntities[static_cast<std::size_t>(i)];
}

int Scene::append(std::unique_ptr<Entity> entity)
{
    entity->setScene(this);
    mEntities.emplace_back(std::move(entity));

    return static_cast<int>(mEntities.size() - 1);
}

void Scene::clear()
{
    mEntities.clear();
}

void Scene::update(const TimeDuration dt)
{
    for (auto& ptr : mEntities)
    {
        ptr->update(dt);
    }
}

void Scene::draw(Renderer& renderer, const float interp)
{
    for (const auto& ptr : mEntities)
    {
        ptr->draw(renderer, interp);
    }
}

} // namespace pong
