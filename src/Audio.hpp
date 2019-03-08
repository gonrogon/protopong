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
/// @file   src/Audio.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Require.hpp"
#include <memory>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an audio system.
///
/// This is a simple audio device made using the audio functions in SDL (no mixer). It only plays a sound, enough
/// for a pong.
////////////////////////////////////////////////////////////
class Audio
{
private:

    /**
     * @brief Define an audio device.
     *
     * It is a wrapper around SDL audio device.
     */
    struct Device;

    /**
     * @brief Define a sound.
     *
     * It handles a buffer to an audio file (WAV).
     */
    struct Sound;

    /** @brief Define a type for unique pointers to audio devices. */
    typedef std::unique_ptr<Device> DevicePtr;

    /** @brief Define a type for unique pointers to sounds. */
    typedef std::unique_ptr<Sound> SoundPtr;

public:

    /**
     * @brief Constructor.
     */
    Audio();

    /**
     * @brief Constructor (copy).
     */
    Audio(const Audio&) = delete;

    /**
     * @brief Destructor.
     */
    ~Audio();

    /**
     * @brief Initialize the audio system.
     *
     * @return True on success; otherwise, false.
     */
    bool init();

    /**
     * @brief Finalize the audio system.
     */
    void quit();

    /**
     * @brief Play the "pong" sound.
     */
    void play();

private:

    /**
     * @brief Load an audio file from memory.
     *
     * @param size Size, in bytes.
     * @param data Read-only pointer to the data.
     */
    bool load(std::size_t size, const void* data);

    /**
     * @brief Audio callback for SDL.
     *
     * @param data   User data (pointer to the instance of the audio system).
     * @param stream Audio buffer to fill.
     * @param length Length of the buffer, in bytes.
     */
    static void callback(void* data, unsigned char* stream, int length);

private:

    /** @brief Audio device. */
    DevicePtr mDevice;

    /** @brief The "pong" sound. */
    SoundPtr mSound;

    /** @brief Flag indicating whether the audio system was initialized or not. */
    bool mError = false;
};

////////////////////////////////////////////////////////////

} // namespace pong
