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
/// @file   src/Audio.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Audio.hpp"
#include "data/Sound.hpp"
#include <algorithm>
#include <cstring>
#include <vector>
#include <SDL.h>

////////////////////////////////////////////////////////////

#define PONG_AUDIO_RESUME 0
#define PONG_AUDIO_PAUSE  1

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

struct Audio::Device
{
    /** @brief Device identifier. */
    SDL_AudioDeviceID mId;

    /** @brief Audio specification. */
    SDL_AudioSpec mSpec;
};

////////////////////////////////////////////////////////////

struct Audio::Sound
{
    /**
     * @return Size, in bytes.
     */
    std::size_t size() const
    {
        return mBuffer.size();
    }

    /**
     * @return Read-only pointer to the data at the current position in the buffer.
     */
    const Uint8* data() const
    {
        return &mBuffer[mPosition];
    }

    /**
     * @return Read position, in bytes, from the beginning of the buffer.
     */
    Uint32 position() const
    {
        return mPosition;
    }

    /**
     * @brief Advance the read position.
     *
     * @param amount Amount, in bytes.
     *
     * @return True on success; otherwise, false.
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
     * @brief Rewind the sound.
     */
    void rewind()
    {
        mPosition = 0;
    }

    /**
     * @brief Allocate memory for a new buffer.
     *
     * @param amount Amount of memory to allocate.
     *
     * @return Read/write pointer to the buffer.
     */
    Uint8* alloc(const std::size_t amount)
    {
        mBuffer.resize(amount, 0);

        return mBuffer.data();
    }

private:

    /** @brief Read position, in bytes. */
    Uint32 mPosition = 0;

    /** @brief Data buffer. */
    std::vector<Uint8> mBuffer;
};

////////////////////////////////////////////////////////////

Audio::Audio()
    :
    mDevice(new Device),
    mSound (new Sound),
    mError (false)
{}

////////////////////////////////////////////////////////////

Audio::~Audio() = default;

////////////////////////////////////////////////////////////

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
        mError = true;
    }
    // Try to load the sound.
    if (!load(data::Sound::SizePong, data::Sound::Pong))
    {
        mError = true;
    }
    // Done.
    return !mError;
}

////////////////////////////////////////////////////////////

void Audio::quit()
{
    if (mDevice)
    {
        SDL_CloseAudioDevice(mDevice->mId);
    }

    mSound .reset();
    mDevice.reset();
}

////////////////////////////////////////////////////////////

void Audio::play()
{
    if (mError)
    {
        return;
    }

    if (SDL_GetAudioDeviceStatus(mDevice->mId) == SDL_AUDIO_PAUSED)
    {
        mSound->rewind();

        SDL_PauseAudioDevice(mDevice->mId, PONG_AUDIO_RESUME);
    }
}

////////////////////////////////////////////////////////////

bool Audio::load(const std::size_t size, const void* mem)
{
    // Audio specification for the WAV file.
    SDL_AudioSpec wavSpec;
    // Length, in bytes.
    Uint32 wavLength = 0;
    // Buffer with the audio data.
    Uint8* wavBuffer = nullptr;
    // Prepare the buffer to be read by SDL.
    SDL_RWops* rw = SDL_RWFromConstMem(mem, static_cast<int>(size));
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

////////////////////////////////////////////////////////////

void Audio::callback(void* data, unsigned char* stream, int length)
{
    Device* device = static_cast<Audio*>(data)->mDevice.get();
    Sound*  sound  = static_cast<Audio*>(data)->mSound .get();
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

////////////////////////////////////////////////////////////

} // namespace pong
