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

#include <chrono>

namespace pong {

/**
 * @brief Defines a real time clock for measuring elapsed time.
 *
 * This clock uses std::chrono for type-safe time measurements.
 */
class RealTimeClock
{
public:

    using Clock = std::chrono::high_resolution_clock;

    using TimePoint = std::chrono::time_point<Clock>;

    using Duration = std::chrono::duration<double>;

    /**
     * @brief Constructor.
     *
     * Starts the clock upon creation.
     */
    RealTimeClock() noexcept;

    RealTimeClock(const RealTimeClock&) = delete;

    RealTimeClock& operator=(const RealTimeClock&) = delete;

    /**
     * @return Time duration elapsed since the clock was started or last restarted.
     */
    [[nodiscard]] Duration elapsed() const noexcept;

    /**
     * @brief Restarts the clock.
     * @return The duration that had elapsed before the restart.
     */
    Duration restart() noexcept;

private:

    /** @brief Start time. */
    TimePoint mStart;
};

} // namespace pong
