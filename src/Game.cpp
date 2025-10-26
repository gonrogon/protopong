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

#include "Game.hpp"
#include "Entity.hpp"
#include "Event.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Label.hpp"
#include "Table.hpp"
#include "ControllerHuman.hpp"
#include "ControllerAI.hpp"
#include "Project.hpp"

namespace pong {

Game::Game(Audio& audio)
    :
    mAudio      (audio),
    mSceneMenus (*this),
    mSceneMatch (*this)
{}

void Game::handle(const Event& event)
{
    // Movement events are send always to the paddles.
    if (mPaddleA && event.isPlayerA()) { mPaddleA->handle(event); }
    if (mPaddleB && event.isPlayerB()) { mPaddleB->handle(event); }

    switch (mState)
    {
        // Main menu.
        case State::Main:
        {
            if (event.is(Event::Type::One))
            {
                mState = State::Match;

                clear();
                setupMatch(1);
            }

            if (event.is(Event::Type::Two))
            {
                mState = State::Match;

                clear();
                setupMatch(2);
            }

            if (event.is(Event::Type::Help))
            {
                mState = State::Help;

                clear();
                setupHelp();
            }

            if (event.is(Event::Type::Quit))
            {
                mState = State::Done;

                clear();
            }
        }
        break;
        // Match.
        case State::Match:
        {
            if (event.is(Event::Type::Win))
            {
                mState = State::Win;

                setupWin();
            }

            if (event.is(Event::Type::Quit) || event.is(Event::Type::Minimize))
            {
                if (mPaddleA) { mPaddleA->handle(Event{Event::Type::Pause}); }
                if (mPaddleB) { mPaddleB->handle(Event{Event::Type::Pause}); }
                if (mBall)    { mBall   ->handle(Event{Event::Type::Pause}); }

                mState = State::Abort;

                setupAbort();
            }
        }
        break;
        // Win scene.
        case State::Win:
        {
            if (event.is(Event::Type::Quit) || event.is(Event::Type::Next))
            {
                mState = State::Main;

                clear();
                setupMain();
            }
        }
        break;
        // Abort message.
        case State::Abort:
        {
            if (event.is(Event::Type::No) || event.is(Event::Type::Quit))
            {
                mState = State::Match;

                clearMenus();
            }

            if (event.is(Event::Type::Yes))
            {
                mState = State::Main;

                clear();
                setupMain();
            }
        }
        break;
        // Kickoff message:
        case State::Kickoff:
        {
            if (event.is(Event::Type::Next) || event.is(Event::Type::Quit))
            {
                mState = State::Match;

                clearMenus();
            }
        }
        break;
        // Help menu.
        case State::Help:
        {
            if (event.is(Event::Type::Quit))
            {
                mState = State::Main;

                clear();
                setupMain();
            }
        }
        break;

        default:;
    }
}

void Game::update(const TimeDuration dt)
{
    // The initial state changes automatically to main.
    if (mState == State::Start)
    {
        mState = State::Main;
        setupMain();
    }
    // Update the scene for the match if a match is up and running.
    if (mState == State::Match)
    {
        mSceneMatch.update(dt);

        if (mBall->point())
        {
            if (mBall->pointPaddleA()) { ++mScoreA; }
            if (mBall->pointPaddleB()) { ++mScoreB; }

            if (mScoreA >= MaxPoints || mScoreB >= MaxPoints)
            {
                handle(Event{Event::Type::Win});
            }
            else
            {
                scorePoints();

                mState = State::Kickoff;
                setupKickoff();
            }
        }
    }
    // Menus are always updated.
    mSceneMenus.update(dt);
}

void Game::draw(Renderer& renderer, const float interp)
{
    mSceneMatch.draw(renderer, interp);
    mSceneMenus.draw(renderer, interp);
}

void Game::setupMain()
{
    mSceneMenus.emplace<Label>(20.0f, glm::vec2{0.0f,  60.0f}, ColorBlue,  "PROTO");
    mSceneMenus.emplace<Label>(20.0f, glm::vec2{0.0f,  35.0f}, ColorBlue,  "PONG");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -20.0f}, ColorWhite, "Press (1) for single player");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -35.0f}, ColorWhite, "Press (2) for player vs player");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -50.0f}, ColorWhite, "Press (h) to view controls");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -65.0f}, ColorWhite, "Press (ESC) to exit");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -90.0f}, ColorGray,  PONG_VERSION);
}

void Game::setupMatch(const int players)
{
    mScoreA = 0;
    mScoreB = 0;

    mTable = mSceneMatch.emplace<Table>(glm::vec2(0.0f, -10.0f), glm::vec2(200.0f, 140.0f));

    const float centerTop   = 100.0f - (100.0f - mTable->top()) * 0.5f;
    const float centerLeft  = mTable->left () - 0.5f * (mTable->left () - mTable->position().x);
    const float centerRight = mTable->right() - 0.5f * (mTable->right() - mTable->position().x);

    mLabelScoreA = mSceneMatch.emplace<Label>(15.0f, glm::vec2{centerRight, centerTop}, ColorWhite, std::to_string(mScoreA));
    mLabelScoreB = mSceneMatch.emplace<Label>(15.0f, glm::vec2{centerLeft,  centerTop}, ColorWhite, std::to_string(mScoreB));

    std::unique_ptr<Controller> ca, cb;
    if (players >= 2)
    {
        ca = std::make_unique<ControllerHuman>(ControllerHuman::Player::A);
        cb = std::make_unique<ControllerHuman>(ControllerHuman::Player::B);
    }
    else
    {
        ca = std::make_unique<ControllerHuman>(ControllerHuman::Player::A);
        cb = std::make_unique<ControllerAI>();
    }

    mPaddleA = mSceneMatch.emplace<Paddle>(std::move(ca), glm::vec2{mTable->right() - 10.0f, mTable->position().y}, glm::vec2{5.0f, 30.0f});
    mPaddleB = mSceneMatch.emplace<Paddle>(std::move(cb), glm::vec2{mTable->left()  + 10.0f, mTable->position().y}, glm::vec2{5.0f, 30.0f});
    mBall    = mSceneMatch.emplace<Ball>(mTable->position(), 2.5f);
    mPaddleA->setup(*mTable, *mBall);
    mPaddleB->setup(*mTable, *mBall);
    mBall   ->setup(*mTable, *mPaddleA, *mPaddleB);
}

void Game::setupWin()
{
    if (mBall->pointPaddleA())
    {
        mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->position().y + 7.5f}, ColorRed, "Right player won!!!");
    }
    else
    {
        mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->position().y + 7.5f}, ColorRed, "Left player won!!!");
    }
    mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->position().y - 7.5f}, ColorRed, "Press (ESC) to exit");
}

void Game::setupAbort()
{
    mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->position().y + 7.5f}, ColorRed, "Are you sure you want to quit?");
    mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->position().y - 7.5f}, ColorRed, "(Y)es  (N)o");
}

void Game::setupKickoff()
{
    mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->top()    - 20}, ColorBlue, "Press (space) to kickoff");
    mSceneMenus.emplace<Label>(5.0f, glm::vec2{0.0f, mTable->bottom() + 20}, ColorBlue, "Press (space) to kickoff");
}

void Game::setupHelp()
{
    mSceneMenus.emplace<Label>(10.0f, glm::vec2{0.0f,  60.0f}, ColorBlue,  "Controls");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f,  20.0f}, ColorGray,  "Right player:");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f,  10.0f}, ColorWhite, "(up arrow) move up, (down arrow) move down");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, - 5.0f}, ColorGray,  "Left player:");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -15.0f}, ColorWhite, "(w) move up, (s) move down");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -30.0f}, ColorGray,  "Both players:");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -40.0f}, ColorWhite, "(space) kickoff");
    mSceneMenus.emplace<Label>( 5.0f, glm::vec2{0.0f, -80.0f}, ColorWhite, "Press (ESC) to return");
}

void Game::scorePoints()
{
    mLabelScoreA->setText(std::to_string(mScoreA));
    mLabelScoreB->setText(std::to_string(mScoreB));

    if (mBall->pointPaddleA()) { mBall->reset(mTable->position(),  InitialSpeed); }
    if (mBall->pointPaddleB()) { mBall->reset(mTable->position(), -InitialSpeed); }

    mPaddleA->setPosition({mTable->right() - 10.0f, mTable->position().y});
    mPaddleA->stop();
    mPaddleB->setPosition({mTable->left()  + 10.0f, mTable->position().y});
    mPaddleB->stop();
}

void Game::clear()
{
    mPaddleA = nullptr;
    mPaddleB = nullptr;
    mBall = nullptr;
    mTable = nullptr;
    mLabelScoreA = nullptr;
    mLabelScoreB = nullptr;
    mSceneMenus.clear();
    mSceneMatch.clear();
}

void Game::clearMenus()
{
    mSceneMenus.clear();
}

} // namespace pong
