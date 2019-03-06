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
/// @file   src/Event.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Require.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an event.
///
/// An event is a wrapper around an event type.
////////////////////////////////////////////////////////////
class Event
{
public:

    /**
     * @brief Define an enumeration with the types of event.
     */
    enum class Type
    {
        PlayerAMoveUp,           //!< Player A move up.
        PlayerAMoveUpReleased,   //!< Player A move up released.
        PlayerAMoveDown,         //!< Player A move down.
        PlayerAMoveDownReleased, //!< Player A move down.
        PlayerBMoveUp,           //!< Player B move up.
        PlayerBMoveUpReleased,   //!< Player B move up.
        PlayerBMoveDown,         //!< Player B move down.
        PlayerBMoveDownReleased, //!< Player B move down.
        Minimize,                //!< Window minimization.
        Maximize,                //!< Window maximization.
        Pause,                   //!< Pause.
        Next,                    //!< Next.
        Quit,                    //!< Quit.
        Yes,                     //!< Yes.
        No,                      //!< No.
        Help,                    //!< Help.
        One,                     //!< One player.
        Two,                     //!< Two players.
        Win,                     //!< Match is over.
    };

    /**
     * @brief Constructor.
     *
     * @param type Type of event.
     */
    explicit Event(Type type);

    /**
     * @return Type.
     */
    Type type() const;

    /**
     * @return True if it is an event of the requested type; otherwise, false.
     */
    bool is(Type type) const;

    /**
     * @return True if it is an event for player A; otherwise, false.
     */
    bool isPlayerA() const;

    /**
     * @return True if it is an event for player B; otherwise, false.
     */
    bool isPlayerB() const;

private:

    /** @brief Type of event. */
    Type mType;
};

////////////////////////////////////////////////////////////

} // namespace pong
