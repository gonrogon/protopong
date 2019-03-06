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
/// @file   src/Game.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
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
#include "RendererGL3.hpp"

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

const int       Game::MaxPoints    = 10;
const float     Game::InitialSpeed = 100.0f;
const glm::vec4 Game::ColorWhite   = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 Game::ColorBlue    = glm::vec4(0.5f, 0.8f, 1.0f, 1.0f);
const glm::vec4 Game::ColorRed     = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
const glm::vec4 Game::ColorGray    = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

////////////////////////////////////////////////////////////

Game::Game(Audio& audio)
    :
    mAudio      (audio),
    mSceneMenus (*this),
    mSceneMatch (*this)
{}

////////////////////////////////////////////////////////////

Audio& Game::audio()
{
    return mAudio;
}

////////////////////////////////////////////////////////////

bool Game::done() const
{
    return mState == State::Done;
}

////////////////////////////////////////////////////////////

void Game::handle(const Event& event)
{
    // Movement events are send always to the paddles.
    if (mPaddleA >= 0 && event.isPlayerA()) { mSceneMatch.at(mPaddleA).handle(event); }
    if (mPaddleB >= 0 && event.isPlayerB()) { mSceneMatch.at(mPaddleB).handle(event); }
    // Handle the states.
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
                if (mPaddleA >= 0) { mSceneMatch.at(mPaddleA).handle(Event{Event::Type::Pause}); }
                if (mPaddleB >= 0) { mSceneMatch.at(mPaddleB).handle(Event{Event::Type::Pause}); }
                if (mBall    >= 0) { mSceneMatch.at(mBall)   .handle(Event{Event::Type::Pause}); }

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

////////////////////////////////////////////////////////////

void Game::update(const float dt)
{
    // The initial state changes automatically.
    if (mState == State::Start)
    {
        mState = State::Main;

        setupMain();
    }
    // Update the scene for the match if a match is up and running.
    if (mState == State::Match)
    {
        mSceneMatch.update(dt);

        auto& ball = static_cast<Ball&>(mSceneMatch.at(mBall));

        if (ball.point())
        {
            if (ball.pointPaddleA()) { ++mScoreA; }
            if (ball.pointPaddleB()) { ++mScoreB; }

            if (mScoreA >= MaxPoints || mScoreB >= MaxPoints)
            {
                handle(Event{Event::Type::Win});
            }
            else
            {
                scorePoints();
            }
        }
    }
    // Menus are always updated.
    mSceneMenus.update(dt);
}

////////////////////////////////////////////////////////////

void Game::draw(const float dt, const float interp, Renderer& renderer)
{
    mSceneMatch.draw(dt, interp, renderer);
    mSceneMenus.draw(dt, interp, renderer);
}

////////////////////////////////////////////////////////////

void Game::setupMain()
{
    mSceneMenus.append(new Label(20.0f, {0.0f,  60.0f}, ColorBlue,  "PROTO"));
    mSceneMenus.append(new Label(20.0f, {0.0f,  35.0f}, ColorBlue,  "PONG"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -20.0f}, ColorWhite, "Press (1) for single player"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -35.0f}, ColorWhite, "Press (2) for player vs player"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -50.0f}, ColorWhite, "Press (h) to view controls"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -65.0f}, ColorWhite, "Press (ESC) to exit"));
}

////////////////////////////////////////////////////////////

void Game::setupMatch(int players)
{
    mScoreA = 0;
    mScoreB = 0;

    Table* const tbl = new Table(glm::vec2(0.0f, -10.0f), glm::vec2(200.0f, 140.0f));

    float tCenter = 100.0f - (100.0f - tbl->top()) * 0.5f;
    float lCenter = tbl->left () - 0.5f * (tbl->left () - tbl->position().x);
    float rCenter = tbl->right() - 0.5f * (tbl->right() - tbl->position().x);

    Label* const lbA = new Label(15.0f, {rCenter, tCenter}, ColorWhite, std::to_string(mScoreA));
    Label* const lbB = new Label(15.0f, {lCenter, tCenter}, ColorWhite, std::to_string(mScoreB));

    mTable       = mSceneMatch.append(tbl);
    mLabelScoreA = mSceneMatch.append(lbA);
    mLabelScoreB = mSceneMatch.append(lbB);

    Controller *ca, *cb;

    if (players >= 2)
    {
        ca = new ControllerHuman();
        cb = new ControllerHuman();
    }
    else
    {
        ca = new ControllerHuman();
        cb = new ControllerAI();
    }

    Paddle* const paddleA = new Paddle(ca, {tbl->right() - 10.0f, tbl->position().y}, {5.0f, 30.0f});
    Paddle* const paddleB = new Paddle(cb, {tbl->left()  + 10.0f, tbl->position().y}, {5.0f, 30.0f});
    Ball*   const ball    = new Ball  (tbl->position(), 2.5f);

    mPaddleA = mSceneMatch.append(paddleA);
    mPaddleB = mSceneMatch.append(paddleB);
    mBall    = mSceneMatch.append(ball);

    paddleA->setup(mTable, mBall);
    paddleB->setup(mTable, mBall);
    ball   ->setup(mTable, mPaddleA, mPaddleB);
}

////////////////////////////////////////////////////////////

void Game::setupWin()
{
    Table& table = static_cast<Table&>(mSceneMatch.at(mTable));
    Label* lbl1  = nullptr;
    Label* lbl2  = nullptr;

    if (static_cast<Ball&>(mSceneMatch.at(mBall)).pointPaddleA())
    {
        lbl1 = new Label(5.0f, {0.0f, table.position().y + 7.5f}, ColorRed, "Right player won!!!");
    }
    else
    {
        lbl1 = new Label(5.0f, {0.0f, table.position().y + 7.5f}, ColorRed, "Left player won!!!");
    }

    lbl2 = new Label(5.0f, {0.0f, table.position().y - 7.5f}, ColorRed, "Press (ESC) to exit");

    mSceneMenus.append(lbl1);
    mSceneMenus.append(lbl2);
}

////////////////////////////////////////////////////////////

void Game::setupAbort()
{
    Table& table = static_cast<Table&>(mSceneMatch.at(mTable));

    mSceneMenus.append(new Label(5.0f, {0.0f, table.position().y + 7.5f}, ColorRed, "Are you sure you want to quit?"));
    mSceneMenus.append(new Label(5.0f, {0.0f, table.position().y - 7.5f}, ColorRed, "(Y)es  (N)o"));
}

////////////////////////////////////////////////////////////

void Game::setupHelp()
{
    mSceneMenus.append(new Label(10.0f, {0.0f,  60.0f}, ColorBlue,  "Controls"));
    mSceneMenus.append(new Label( 5.0f, {0.0f,  20.0f}, ColorWhite, "Right player:"));
    mSceneMenus.append(new Label( 5.0f, {0.0f,   5.0f}, ColorWhite, "(up arrow) move up, (down arrow) move down"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -10.0f}, ColorWhite, "Left player:"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -25.0f}, ColorWhite, "(w) move up, (s) move down"));
    mSceneMenus.append(new Label( 5.0f, {0.0f, -60.0f}, ColorWhite, "Press (ESC) to return"));
}

////////////////////////////////////////////////////////////

void Game::scorePoints()
{
    Table& table = static_cast<Table&>(mSceneMatch.at(mTable));
    Ball&  ball  = static_cast<Ball&> (mSceneMatch.at(mBall));

    static_cast<Label&>(mSceneMatch.at(mLabelScoreA)).setText(std::to_string(mScoreA));
    static_cast<Label&>(mSceneMatch.at(mLabelScoreB)).setText(std::to_string(mScoreB));

    if (ball.pointPaddleA()) { ball.reset(table.position(),  InitialSpeed); }
    if (ball.pointPaddleB()) { ball.reset(table.position(), -InitialSpeed); }
}

////////////////////////////////////////////////////////////

void Game::clear()
{
    mPaddleA = -1;
    mPaddleB = -1;

    mSceneMenus.clear();
    mSceneMatch.clear();
}

////////////////////////////////////////////////////////////

void Game::clearMenus()
{
    mSceneMenus.clear();
}

////////////////////////////////////////////////////////////

} // namespace pong
