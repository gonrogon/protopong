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

#include <memory>

namespace pong {

/**
 * @brief Manages loading and playback of WAV audio using the SDL audio subsystem.
 *
 * This class is a self-contained audio engine designed for playing a single pre-loaded sound effect.
 */
class Audio
{
    /** @brief Define a wrapper for a SDL audio device. */
    struct Device;

    /** @brief Define a wrapper for a SDL sound. */
    struct Sound;

public:

    /**
     * @brief Factory method to create and initialize the audio system.
     * @return A unique pointer holding the new instance if initialization is successful, or null if it fails.
     */
    [[nodiscard]] static std::unique_ptr<Audio> create();

    Audio(const Audio&) = delete;

    Audio(Audio&&) = delete;

    Audio& operator=(const Audio&) = delete;

    Audio& operator=(Audio&&) = delete;

    ~Audio();

private:

    /**
     * @brief Constructor.
     */
    Audio();

    /**
     * @brief Internal initialization method called by the factory.
     * @return True on success, false otherwise.
     */
    bool init();

public:

    /**
     * @brief Play the "pong" sound.
     */
    void play();

private:

    /**
     * @brief Loads and converts a WAV file from memory.
     * @param size Size of the memory buffer, in bytes.
     * @param data Read-only pointer to the memory buffer.
     * @return True on success, false otherwise.
     */
    bool load(std::size_t size, const void* data);

    /**
     * @brief C-style callback function passed to SDL audio.
     *
     * @param data User data (pointer to the instance of the audio system).
     * @param stream Audio buffer to fill.
     * @param length Length of the buffer, in bytes.
     */
    static void callback(void* data, unsigned char* stream, int length);

private:

    /** @brief Audio device. */
    std::unique_ptr<Device> mDevice;

    /** @brief The "pong" sound. */
    std::unique_ptr<Sound> mSound;
};

} // namespace pong
