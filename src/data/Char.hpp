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

#include <cstdint>
#include <array>
#include <span>

namespace pong::data::font {

/** @brief Max character width. */
inline constexpr unsigned char MaxWidth = 7;

/** @brief Max character height. */
inline constexpr unsigned char MaxHeight = 7;

/** @brief Character advance. */
inline constexpr unsigned char Advance = 9;

/**
 * @brief Gets a view of the data for the glyph of a specific character.
 * @param codepoint Character codepoint.
 * @return Span with the data of the glyph if it is found, an empty span otherwise.
 */
[[nodiscard]] std::span<const unsigned char> getGlyph(std::uint32_t codepoint);

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
inline constexpr std::array<std::array<unsigned char, 42>, 44> Glyphs =
{{
    // A
    {6, 7, 0, 0, 2, 5, 2, 2, 3, 1, 5, 0, 2, 5, 1, 5, 2, 1, 2, 6, 3, 1, 4, 5, 2, 1},
    // B
    {6, 7, 0, 0, 2, 7, 2, 0, 4, 1, 2, 3, 4, 1, 2, 6, 4, 1, 5, 4, 2, 2, 5, 1, 2, 2},
    // C
    {7, 7, 0, 2, 2, 3, 1, 1, 2, 1, 2, 0, 4, 1, 5, 1, 2, 1, 1, 5, 2, 1, 2, 6, 4, 1, 5, 5, 2, 1},
    // D
    {6, 7, 0, 0, 2, 7, 2, 0, 3, 1, 4, 1, 2, 1, 5, 2, 2, 3, 4, 5, 2, 1, 2, 6, 3, 1},
    // E
    {4, 7, 0, 0, 2, 7, 2, 0, 5, 1, 2, 3, 4, 1, 2, 6, 5, 1},
    // F
    {3, 7, 0, 0, 2, 7, 2, 3, 4, 1, 2, 6, 5, 1},
    // G
    {7, 7, 0, 2, 2, 3, 1, 1, 2, 1, 2, 0, 5, 1, 5, 1, 2, 2, 4, 3, 3, 1, 1, 5, 2, 1, 2, 6, 5, 1},
    // H
    {3, 7, 0, 0, 2, 7, 2, 3, 3, 1, 5, 0, 2, 7},
    // I
    {3, 6, 0, 0, 6, 1, 2, 1, 2, 5, 0, 6, 6, 1},
    // J
    {3, 7, 0, 1, 2, 1, 1, 0, 5, 1, 5, 1, 2, 6},
    // K
    {8, 7, 0, 0, 2, 7, 2, 2, 2, 2, 4, 2, 1, 1, 3, 1, 3, 1, 4, 0, 3, 1, 3, 4, 2, 1, 4, 5, 2, 1, 5, 6, 2, 1},
    // L
    {2, 7, 0, 0, 2, 7, 2, 0, 5, 1},
    // M
    {5, 7, 0, 0, 2, 7, 2, 3, 1, 3, 3, 2, 1, 3, 4, 3, 1, 3, 5, 0, 2, 7},
    // N
    {5, 7, 0, 0, 2, 7, 2, 3, 1, 3, 3, 2, 1, 3, 4, 1, 1, 3, 5, 0, 2, 7},
    // O
    {4, 7, 0, 1, 2, 5, 1, 0, 5, 1, 5, 1, 2, 5, 1, 6, 5, 1},
    // P
    {4, 7, 0, 0, 2, 7, 2, 2, 4, 1, 2, 6, 4, 1, 5, 3, 2, 3},
    // Q
    {7, 7, 0, 1, 2, 5, 1, 0, 4, 1, 3, 2, 2, 1, 4, 1, 2, 1, 6, 0, 1, 1, 5, 2, 2, 4, 1, 6, 5, 1},
    // R
    {7, 7, 0, 0, 2, 7, 2, 6, 4, 1, 5, 4, 2, 2, 4, 3, 3, 1, 2, 2, 3, 1, 3, 1, 3, 1, 4, 0, 3, 1},
    // S
    {7, 7, 0, 1, 2, 1, 1, 0, 5, 1, 5, 1, 2, 2, 1, 3, 5, 1, 0, 4, 2, 2, 1, 6, 5, 1, 5, 5, 2, 1},
    // T
    {2, 6, 0, 6, 6, 1, 2, 0, 2, 6},
    // U
    {3, 7, 0, 1, 2, 6, 1, 0, 5, 1, 5, 1, 2, 6},
    // V
    {7, 7, 0, 4, 2, 3, 0, 3, 3, 1, 1, 2, 5, 1, 2, 1, 3, 1, 3, 0, 1, 1, 4, 3, 3, 1, 5, 4, 2, 3},
    // W
    {5, 7, 0, 0, 2, 7, 2, 1, 1, 3, 3, 2, 1, 3, 4, 1, 1, 3, 5, 0, 2, 7},
    // X
    {9, 7, 0, 0, 2, 2, 5, 0, 2, 2, 5, 5, 2, 2, 0, 5, 2, 2, 1, 4, 5, 1, 1, 2, 5, 1, 2, 1, 1, 5, 4, 1, 1, 5, 2, 2, 3, 3},
    // Y
    {4, 6, 0, 4, 2, 3, 1, 3, 4, 1, 2, 0, 2, 3, 4, 4, 2, 3},
    // Z
    {7, 7, 0, 6, 7, 1, 4, 5, 3, 1, 3, 4, 3, 1, 2, 3, 3, 1, 1, 2, 3, 1, 0, 1, 3, 1, 0, 0, 7, 1},
    // 0
    {8, 7, 0, 2, 2, 3, 1, 1, 2, 1, 2, 0, 3, 1, 5, 1, 1, 1, 5, 2, 2, 3, 4, 5, 2, 1, 2, 6, 3, 1, 1, 5, 1, 1},
    // 1
    {3, 6, 0, 0, 6, 1, 2, 1, 2, 6, 1, 5, 1, 1},
    // 2
    {8, 7, 0, 5, 2, 1, 1, 6, 5, 1, 5, 5, 2, 1, 4, 4, 3, 1, 2, 3, 4, 1, 1, 2, 4, 1, 0, 1, 3, 1, 0, 0, 7, 1},
    // 3
    {7, 7, 1, 6, 6, 1, 4, 5, 2, 1, 3, 4, 2, 1, 2, 3, 4, 1, 5, 1, 2, 2, 1, 0, 5, 1, 0, 1, 2, 1},
    // 4
    {6, 7, 4, 0, 2, 7, 3, 6, 1, 1, 2, 5, 2, 1, 1, 4, 2, 1, 0, 3, 2, 1, 0, 2, 7, 1},
    // 5
    {6, 7, 0, 6, 6, 1, 0, 5, 2, 1, 0, 4, 6, 1, 5, 1, 2, 3, 1, 0, 5, 1, 0, 1, 2, 1},
    // 6
    {6, 7, 2, 6, 4, 1, 1, 5, 2, 1, 0, 1, 2, 4, 1, 0, 5, 1, 5, 1, 2, 2, 2, 3, 4, 1},
    // 7
    {6, 7, 0, 5, 2, 1, 0, 6, 7, 1, 5, 5, 2, 1, 4, 4, 2, 1, 3, 3, 2, 1, 2, 0, 2, 3},
    // 8
    {8, 7, 5, 4, 1, 2, 1, 6, 4, 1, 0, 4, 2, 2, 1, 3, 2, 2, 3, 2, 2, 2, 5, 1, 2, 2, 1, 0, 5, 1, 0, 1, 1, 2},
    // 9
    {6, 7, 1, 3, 4, 1, 0, 4, 2, 2, 1, 6, 5, 1, 5, 2, 2, 4, 4, 1, 2, 1, 1, 0, 4, 1},
    // :
    {2, 2, 0, 0, 2, 2, 0, 4, 2, 2},
    // !
    {3, 7, 2, 0, 2, 1, 2, 2, 2, 2, 2, 4, 3, 3},
    // ?
    {6, 7, 0, 4, 2, 2, 1, 5, 5, 2, 5, 4, 2, 2, 4, 3, 2, 1, 2, 2, 3, 1 ,2, 0, 3, 1},
    // (
    {5, 4, 2, 6, 2, 1, 1, 5, 2, 1, 0, 2, 2, 3, 1, 1, 2, 1, 2, 0, 2, 1},
    // )
    {5, 4, 0, 6, 2, 1, 1, 5, 2, 1, 2, 2, 2, 3, 1, 1, 2, 1, 0, 0, 2, 1},
    // Á
    {8, 7, 0, 0, 2, 5, 2, 2, 3, 1, 5, 0, 2, 5, 1, 5, 2, 1, 2, 6, 3, 1, 4, 5, 2, 1, 2, 8, 2, 1, 3, 9, 2, 1},
    // ,
    {2, 3, 0, 0, 2, 1, 1, 1, 2, 2},
    // .
    {1, 3, 0, 0, 2, 2}
}};

} // namespace pong::data::font
