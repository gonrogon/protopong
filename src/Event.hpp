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

namespace pong {

/**
 * @brief Defines an event.
 *
 * This class acts as a wrapper around an enumeration of possible game events. It provides a clear and type-safe way to
 * handle user input and game state changes, such as player movements, window events, or menu selections.
 */
class Event
{
public:

    /**
     * @brief Define an enumeration with all possible types of events that can occur in the game.
     */
    enum class Type
    {
        PlayerAMoveUp,           //!< Player A pressed move up.
        PlayerAMoveUpReleased,   //!< Player A released move up.
        PlayerAMoveDown,         //!< Player A pressed move down.
        PlayerAMoveDownReleased, //!< Player A released move down.
        PlayerBMoveUp,           //!< Player B pressed move up.
        PlayerBMoveUpReleased,   //!< Player B released move up.
        PlayerBMoveDown,         //!< Player B pressed move down.
        PlayerBMoveDownReleased, //!< Player B released move down.
        Minimize,                //!< The window was minimized.
        Maximize,                //!< The window was maximized.
        Pause,                   //!< The game was paused.
        Next,                    //!< A confirmation or "next screen" action was triggered.
        Quit,                    //!< The user request to quit the game.
        Yes,                     //!< A "yes" option was selected in a prompt.
        No,                      //!< A "no" option was selected in a prompt.
        Help,                    //!< The help screen was requested.
        One,                     //!< The one-player mode was selected.
        Two,                     //!< The two-player mode was selected.
        Win,                     //!< A player has won the match, ending the game.
    };

    /**
     * @brief Constructor.
     * @param type The specific type of the event.
     */
    explicit Event(const Type type) noexcept : mType(type) {}

    /**
     * @brief Gets the type of the event.
     * @return Type.
     */
    [[nodiscard]] Type type() const noexcept { return mType; }

    /**
     * @brief Checks if the event is of a specific type.
     * @param type The type to compare against.
     * @return True if it is an event of the requested type; otherwise, false.
     */
    [[nodiscard]] bool is(const Type type) const noexcept { return mType == type; }

    /**
     * @brief Checks if the event is related to Player A's actions.
     * @return True if it is an event of player A; otherwise, false.
     */
    [[nodiscard]] bool isPlayerA() const noexcept;

    /**
     * @brief Checks if the event is related to Player A's actions.
     * @return True if it is an event of player B; otherwise, false.
     */
    [[nodiscard]] bool isPlayerB() const noexcept;

private:

    /** @brief Type of this event. */
    Type mType;
};

} // namespace pong
