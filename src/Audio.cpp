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

#include "Audio.hpp"
#include "data/Sound.hpp"
#include <algorithm>
#include <cstring>
#include <vector>
#include <SDL.h>

#define PONG_AUDIO_RESUME 0
#define PONG_AUDIO_PAUSE  1

namespace pong {

/**
 * @brief Structure that encapsulates all SDL-related audio device data.
 *
 * This structure holds the unique identifier for the opened audio device and a copy of the final `SDL_AudioSpec` that
 * the hardware is using, which may differ from the one requested.
 * Its lifetime is managed by the parent `Audio` class.
 */
struct Audio::Device
{
    /** @brief Unique ID for the opened SDL audio device. A value of 0 indicates an error or uninitialized state. */
    SDL_AudioDeviceID mId;

    /** @brief Audio specification (frequency, format, etc.) that the device was actually opened with. */
    SDL_AudioSpec mSpec;
};

/**
 * @brief Structure that holds the raw, converted audio data and playback state.
 *
 * This structure contains the audio sample data in a vector, converted to the format required by the audio device. It
 * also tracks the current playback position within that buffer.
 */
struct Audio::Sound
{
    /**
     * @brief Gets the total size of the audio buffer, in bytes.
     * @return The size of the buffer.
     */
    std::size_t size() const { return mBuffer.size(); }

    /**
     * @brief Gets a pointer to the current playback position within the audio buffer.
     * @return A read-only pointer to the data at the current position.
     */
    const Uint8* data() const { return &mBuffer[mPosition]; }

    /**
     * @brief Gets the current playback position in bytes from the beginning of the buffer.
     * @return The current position.
     */
    Uint32 position() const { return mPosition; }

    /**
     * @brief Advances the playback position by a given amount.
     * @details Checks for buffer boundaries to prevent reading past the end of the sound.
     * @param amount The number of bytes to advance.
     * @return True if the new position is valid, false if the end of the sound was reached.
     */
    bool advance(const Uint32 amount)
    {
        const Uint32 next = mPosition + amount;
        if (next >= size())
        {
            return false;
        }

        mPosition = next;
        return true;
    }

    /**
     * @brief Resets the playback position to the beginning of the sound.
     */
    void rewind() { mPosition = 0; }

    /**
     * @brief Allocates (or reallocates) the internal buffer to a new size.
     * @param amount The desired size of the buffer in bytes.
     * @return A raw, writable pointer to the beginning of the newly allocated buffer.
     */
    Uint8* alloc(const std::size_t amount)
    {
        mBuffer.resize(amount, 0);
        return mBuffer.data();
    }

private:

    /** @brief The raw audio data, converted to the device's format. */
    std::vector<Uint8> mBuffer;

    /** @brief The current read position within the buffer for playback, in bytes. */
    Uint32 mPosition = 0;
};

std::unique_ptr<Audio> Audio::create()
{
    auto audio = std::unique_ptr<Audio>(new Audio{});
    if (audio->init())
    {
        return audio;
    }

    return nullptr;
}


Audio::Audio()
{
    mDevice = std::make_unique<Device>();
    mSound  = std::make_unique<Sound>();
}

Audio::~Audio()
{
    if (mDevice->mId != 0)
    {
        SDL_LockAudioDevice (mDevice->mId);
        SDL_CloseAudioDevice(mDevice->mId);
    }
}

bool Audio::init()
{
    SDL_AudioSpec want;
    // Set the desired audio specification.
    SDL_zero(want);
    want.freq     = 44100;
    want.format   = AUDIO_F32;
    want.channels = 1;
    want.samples  = 4096;
    want.callback = &Audio::callback;
    want.userdata = this;
    // Try to open the audio device.
    if ((mDevice->mId = SDL_OpenAudioDevice(nullptr, 0, &want, &mDevice->mSpec, 0)) == 0)
    {
        return false;
    }
    // Try to load the sound.
    if (!load(data::PongSound.size(), data::PongSound.data()))
    {
        return false;
    }

    return true;
}

void Audio::play()
{
    if (!mDevice || mDevice->mId == 0)
    {
        return;
    }

    // Lock the device to safety modify the state of the sound.
    SDL_LockAudioDevice(mDevice->mId);
    mSound->rewind();
    SDL_UnlockAudioDevice(mDevice->mId);

    SDL_PauseAudioDevice(mDevice->mId, PONG_AUDIO_RESUME);
}

bool Audio::load(const std::size_t size, const void* data)
{
    // Audio specification for the WAV file.
    SDL_AudioSpec wavSpec;
    // Length, in bytes.
    Uint32 wavLength = 0;
    // Buffer with the audio data.
    Uint8* wavBuffer = nullptr;
    // Prepare the buffer to be read by SDL.
    SDL_RWops* rw = SDL_RWFromConstMem(data, static_cast<int>(size));
    if (!rw)
    {
        return false;
    }
    // Try to load the audio file.
    if (!SDL_LoadWAV_RW(rw, 1, &wavSpec, &wavBuffer, &wavLength))
    {
        return false;
    }
    // Convert the WAV to the device format.
    SDL_AudioCVT cvt;
    // Build the audio conversion structure.
    SDL_BuildAudioCVT(&cvt,
               wavSpec.format,        wavSpec.channels,        wavSpec.freq,
        mDevice->mSpec.format, mDevice->mSpec.channels, mDevice->mSpec.freq
    );
    // Allocate memory for the converted audio.
    cvt.len = static_cast<int>(wavLength);
    cvt.buf = mSound->alloc(wavLength * static_cast<unsigned int>(cvt.len_mult));
    // Copy the original WAV to the buffer.
    std::memcpy(cvt.buf, wavBuffer, wavLength);
    // The original WAV can be deleted now.
    SDL_FreeWAV(wavBuffer);
    // Do the conversion.
    SDL_ConvertAudio(&cvt);
    // Done.
    return true;
}

void Audio::callback(void* data, unsigned char* stream, int length)
{
    Device* device = static_cast<Audio*>(data)->mDevice.get();
    Sound*  sound  = static_cast<Audio*>(data)->mSound.get();
    if (!device || !sound)
    {
        return;
    }
    // Reset the buffer.
    std::memset(stream, 0, static_cast<std::size_t>(length));
    // Calculate the remaining data.
    length = std::min(static_cast<int>(sound->size() - sound->position()), length);
    // Mix the sound data into the buffer.
    SDL_MixAudioFormat(stream, sound->data(), device->mSpec.format, static_cast<Uint32>(length), SDL_MIX_MAXVOLUME);
    // Move the read position marker and check if the sound has been played completely.
    if (!sound->advance(static_cast<Uint32>(length)))
    {
        SDL_PauseAudioDevice(device->mId, PONG_AUDIO_PAUSE);
    }
}

} // namespace pong
