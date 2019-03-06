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
/// @file   src/Scene.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Scene.hpp"
#include "Entity.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

Scene::Scene(Game& game)
    :
    mGame(game)
{}

////////////////////////////////////////////////////////////

Scene::~Scene()
{
    clear();
}

////////////////////////////////////////////////////////////

Game& Scene::game() const
{
    return mGame;
}

////////////////////////////////////////////////////////////

Entity& Scene::at(const int i)
{
    return *mEntities.at(i);
}

////////////////////////////////////////////////////////////

int Scene::append(Entity* entity)
{
    entity->setScene(*this);

    mEntities.emplace_back(entity);

    return static_cast<int>(mEntities.size() - 1);
}

////////////////////////////////////////////////////////////

void Scene::clear()
{
    mEntities.clear();
}

////////////////////////////////////////////////////////////

void Scene::update(const float dt)
{
    for (auto& ptr : mEntities)
    {
        ptr->update(dt);
    }
}

////////////////////////////////////////////////////////////

void Scene::draw(const float dt, const float interp, Renderer& renderer)
{
    for (auto& ptr : mEntities)
    {
        ptr->draw(dt, interp, renderer);
    }
}

////////////////////////////////////////////////////////////

} // namespace pong
