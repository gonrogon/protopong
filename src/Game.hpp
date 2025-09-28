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

#include "RealTimeClock.hpp"
#include "Scene.hpp"
#include <glm/glm.hpp>

namespace pong {

class Audio;
class Event;
class Table;
class Paddle;
class Ball;
class Label;

/**
 * @brief Defines a game class that orchestrates the entire game flow, acting as the central state machine.
 *
 * This class is the core of the application logic. It is responsible for:
 * - Managing the overall game state (e.g., main menu, match, win screen).
 * - Owning and managing the game scenes (`mSceneMatch` and `mSceneMenus`).
 * - Creating all game entities and placing them in the appropriate scenes.
 * - Handling and dispatching events to drive state transitions and player actions.
 * - Storing game-wide state, such as player scores.
 * - Maintaining non-owning pointers to critical entities for fast, direct access.
 */
class Game
{
    /** @brief Maximum number of points per match. */
    static constexpr int MaxPoints = 10;

    /** @brief Ball initial speed. */
    static constexpr float InitialSpeed = 100.0f;

    /** @brief White color for common elements. */
    static constexpr auto ColorWhite = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    /** @brief Blue color for texts. */
    static constexpr auto ColorBlue = glm::vec4(0.5f, 0.8f, 1.0f, 1.0f);

    /** @brief Red color for texts. */
    static constexpr auto ColorRed = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);

    /** @brief Gray color for texts. */
    static constexpr auto ColorGray = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

    /**
     * @brief Defines an enumeration with the game states.
     */
    enum class State
    {
        Start   = 0, //!< Initial state before the first frame.
        Main    = 1, //!< The main menu is active.
        Match   = 2, //!< A match is currently in progress.
        Win     = 3, //!< The win/game-over screen is displayed.
        Abort   = 4, //!< The "are you sure you want to quit?" prompt is shown.
        Done    = 5, //!< The game has finished and the application should close.
        Help    = 6, //!< The help/controls screen is displayed.
        Kickoff = 7  //!< The pre-round "kickoff" prompt is shown.
    };

public:

    /**
     * @brief Constructor.
     * @param audio A reference to the audio system for playing sounds.
     */
    explicit Game(Audio& audio);

    Game(const Game&) = delete;

    Game(Game&&) = delete;

    Game& operator=(const Game&) = delete;

    Game& operator=(Game&&) = delete;

    /**
     * @brief Gets a reference to the audio system.
     * @return A reference to the audio object.
     */
    [[nodiscard]] Audio& audio() const noexcept { return mAudio; }

    /**
     * @brief Checks if the game has finished and the application should exit.
     * @return True if the game is finished, false otherwise.
     */
     [[nodiscard]] bool done() const noexcept { return mState == State::Done; }

    /**
     * @brief Handles incoming game events, driving state transitions and player input.
     * @param event Event to handle.
     */
    void handle(const Event& event);

    /**
     * @brief Updates the game.
     *
     * This drives the active scene's update loop and handles game logic like scoring.
     * @param dt The time elapsed since the last update (delta time).
     */
    void update(RealTimeClock::Duration dt);

    /**
     * @brief Draws the current game state to the screen.
     *
     * @param renderer The renderer to use for drawing operations.
     * @param interp The interpolation value for smooth rendering.
     */
    void draw(Renderer& renderer, float interp);

private:

    /** @brief Sets up the scene for the main menu. */
    void setupMain();

    /**
     * @brief Sets up the scene for a match.
     * @param players Number of players (1 or 2).
     */
    void setupMatch(int players);

    /** @brief Sets up the scene for the winning screen. */
    void setupWin();

    /** @briefs Set up the scene for the abort screen. */
    void setupAbort();

    /** @brief Sets up the scene for the kickoff prompt. */
    void setupKickoff();

    /** @brief Set up the scene for the help screen. */
    void setupHelp();

    /** @brief Handles logic after a point is scored (updates scores, resets entities). */
    void scorePoints();

    /** @brief Resets the game to a clean state for a new match or returning to the menu. */
    void clear();

    /** @brief Clears only the menu scene. */
    void clearMenus();

private:

    /** @brief A reference to the audio system. */
    Audio& mAudio;

    /** @brief  The current state of the game's state machine. */
    State mState = State::Start;

    /** @brief Scene for the menus. */
    Scene mSceneMenus;

    /** @brief Scene for the match. */
    Scene mSceneMatch;

    /** @brief  A non-owning pointer to the Player A paddle. Null if no match is active. */
    Paddle* mPaddleA = nullptr;

    /** @brief A non-owning pointer to the Player B paddle. Null if no match is active. */
    Paddle* mPaddleB = nullptr;

    /** @brief A non-owning pointer to the ball. Null if no match is active. */
    Ball* mBall = nullptr;

    /** @brief A non-owning pointer to the table. Null if no match is active. */
    Table* mTable = nullptr;

    /** @brief A non-owning pointer to the Player A score label. Null if no match is active. */
    Label* mLabelScoreA = nullptr;

    /** @brief A non-owning pointer to the Player B score label. Null if no match is active. */
    Label* mLabelScoreB = nullptr;

    /** @brief Score of player A. */
    int mScoreA = 0;

    /** @brief Score of player B. */
    int mScoreB = 0;
};

} // namespace pong
