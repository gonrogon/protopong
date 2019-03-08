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
/// @file   src/Game.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Scene.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define a game.
///
/// This class handles the game state and the scenes.
////////////////////////////////////////////////////////////
class Game
{
private:

    /** @brief Maximum number of points per match. */
    static const int MaxPoints;

    /** @brief Ball initial speed. */
    static const float InitialSpeed;

    /** @brief White color for common elements. */
    static const glm::vec4 ColorWhite;

    /** @brief Blue color for texts. */
    static const glm::vec4 ColorBlue;

    /** @brief Red color for texts. */
    static const glm::vec4 ColorRed;

    /** @brief Gray color for texts. */
    static const glm::vec4 ColorGray;

    /**
     * @brief Defines an enumeration with the game states.
     */
    enum class State
    {
        Start   = 0, //!< Game start.
        Main    = 1, //!< Main menu.
        Match   = 2, //!< Match running.
        Win     = 3, //!< The match is over.
        Abort   = 4, //!< Abort match.
        Done    = 5, //!< Finished.
        Help    = 6, //!< Help.
        Kickoff = 7  //!< Kickoff.
    };

public:

    /**
     * @brief Constructor.
     *
     * @param audio    Audio.
     */
    explicit Game(Audio& audio);

    /**
     * @brief Constructor (copy).
     */
    Game(const Game&) = delete;

    /**
     * @return Audio system.
     */
    Audio& audio() { return mAudio; }

    /**
     * @return True if the game has finished; otherwise, false.
     */
    bool done() const { return mState == State::Done; }

    /**
     * @brief Handle an event.
     *
     * @param event Event to handle.
     */
    void handle(const Event& event);

    /**
     * @brief Update the game.
     *
     * @param dt Time elapsed since the last update.
     */
    void update(float dt);

    /**
     * @brief Draw the game.
     *
     * @param dt     Time elapsed since the last draw.
     * @param interp Interpolation.
     * @param renderer Renderer.
     */
    void draw(float dt, float interp, Renderer& renderer);

private:

    /**
     * @brief Set up the scene for the main menu.
     */
    void setupMain();

    /**
     * @brief Set up the scene for a match.
     *
     * @param player Number of players (1 or 2).
     */
    void setupMatch(int players);

    /**
     * @brief Set up the scene for the winning screen.
     */
    void setupWin();

    /**
     * @brief Set up the scene for the abort screen.
     */
    void setupAbort();

    void setupKickoff();

    /**
     * @brief Set up the scene for the help screen.
     */
    void setupHelp();

    /**
     * @brief Score the points.
     */
    void scorePoints();

    /**
     * @brief Clear the game.
     */
    void clear();

    /**
     * @brief Clear the menus.
     */
    void clearMenus();

private:

    /** @brief Audio system. */
    Audio& mAudio;

    /** @brief Game state. */
    State mState = State::Start;

    /** @brief Scene for the menus. */
    Scene mSceneMenus;

    /** @brief Scene for the match. */
    Scene mSceneMatch;

    /** @brief Identifier of the paddle A. */
    int mPaddleA = -1;

    /** @brief Identifier of the paddle B. */
    int mPaddleB = -1;

    /** @brief Identifier of the ball. */
    int mBall = -1;

    /** @brief Identifier of the table. */
    int mTable = -1;

    /** @brief Identifier of the label for the score of player A. */
    int mLabelScoreA = -1;

    /** @brief Identifier of the label for the score of player B. */
    int mLabelScoreB = -1;

    /** @brief Score of player A. */
    int mScoreA = 0;

    /** @brief Score of player B. */
    int mScoreB = 0;
};

////////////////////////////////////////////////////////////

} // namespace pong
