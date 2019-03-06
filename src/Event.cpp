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
/// @file   src/Event.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Event.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

Event::Event(const Type type)
    :
    mType(type)
{}

////////////////////////////////////////////////////////////

Event::Type Event::type() const
{
    return mType;
}

////////////////////////////////////////////////////////////

bool Event::is(const Type type) const
{
    return mType == type;
}

////////////////////////////////////////////////////////////

bool Event::isPlayerA() const
{
    return mType == Type::PlayerAMoveUp   || mType == Type::PlayerAMoveUpReleased ||
           mType == Type::PlayerAMoveDown || mType == Type::PlayerAMoveDownReleased;
}

////////////////////////////////////////////////////////////

bool Event::isPlayerB() const
{
    return mType == Type::PlayerBMoveUp   || mType == Type::PlayerBMoveUpReleased ||
           mType == Type::PlayerBMoveDown || mType == Type::PlayerBMoveDownReleased;
}

////////////////////////////////////////////////////////////

} // namespace pong
