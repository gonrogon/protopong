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
/// @file   src/data/Char.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include <cstdint>

////////////////////////////////////////////////////////////

namespace pong {
namespace data {

////////////////////////////////////////////////////////////
/// @brief Define a class that contains the glyphs for a simple vector font and the methods to access them.
////////////////////////////////////////////////////////////
class Char
{
public:

    /** @brief Character width. */
    static const unsigned char MaxWidth  = 7;

    /** @brief Character height. */
    static const unsigned char MaxHeight = 7;

    /** @brief Character advance. */
    static const unsigned char Advance = 9;

private:

    /**
     * @brief Array with the glyphs.
     *
     * These glyphs describe the characters as a group of quads with the following format:
     *     {number of quads, width, quad1, quad2, quad3...}
     * Each quad has the following format:
     *     {x-origin, y-origin, width, height}
     * Example for letter "o":
     *     {4, 7, 0, 1, 2, 5, 1, 0, 5, 1, 5, 1, 2, 5, 1, 6, 5, 1}
     *      n  w |^ QUAD 1 ^||^ QUAD 2 ^||^ QUAD 3 ^||^ QUAD 4 ^|
     */
    static const unsigned char Glyphs[43][42];

public:

    /**
     * @brief Constructor.
     */
    Char() = delete;

    /**
     * @brief Get the glyph of a character.
     *
     * @param c Character.
     *
     * @return Read-only pointer to the glyph on success; otherwise, null.
     */
    static const unsigned char* glyph(std::uint32_t c);
};

////////////////////////////////////////////////////////////

} // namespace data
} // namespace pong
