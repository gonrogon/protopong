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
/// @file   src/RealTimeClock.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Require.hpp"
#include <SDL.h>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define a real time clock.
////////////////////////////////////////////////////////////
class RealTimeClock
{
public:

    /**
     * @brief Constructor.
     *
     * Restarts the clock.
     */
    RealTimeClock();

    /**
     * @brief Constructor (copy).
     */
    RealTimeClock(const RealTimeClock&) = delete;

    /**
     * @brief Assignment operator (copy).
     */
    RealTimeClock& operator =(const RealTimeClock&) = delete;

    /**
     * @return Time elapsed since the last restart, in seconds.
     */
    double elapsed() const;

    /**
     * @brief Restart the clock.
     *
     * @return Time elapsed since the last restart, in seconds.
     */
    double restart();

private:

    /** @brief Start time. */
    Uint64 mStart = 0;
};

////////////////////////////////////////////////////////////

} // namespace pong
