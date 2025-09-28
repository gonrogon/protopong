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

#include "data/Char.hpp"

namespace pong::data::font {

[[nodiscard]] std::span<const unsigned char> getGlyph(const std::uint32_t codepoint)
{
    int index = -1;
    if (codepoint >= 'A'  && codepoint <= 'Z')  { index = static_cast<int>(codepoint - 'A'); }
    if (codepoint >= 'a'  && codepoint <= 'z')  { index = static_cast<int>(codepoint - 'a'); }
    if (codepoint >= '0'  && codepoint <= ':')  { index = static_cast<int>(codepoint - '0' + 26); }
    if (codepoint >= '('  && codepoint <= ')')  { index = static_cast<int>(codepoint - '(' + 39); }
    if (codepoint == '!') { index = 37; }
    if (codepoint == '?') { index = 38; }
    if (codepoint == ',') { index = 42; }
    if (codepoint == '.') { index = 43; }
    if (codepoint == 0xc1 || codepoint == 0xe1) { index = 41; }

    if (index < 0 || static_cast<std::size_t>(index) >= Glyphs.size())
    {
        return {};
    }
    // Calculate the number of ite in the row.
    const auto& row = Glyphs[index];
    const std::size_t numQuads = row[0];
    const std::size_t dataSize = 2 + numQuads * 4;

    return {row.begin(), dataSize};
}

} // namespace pong::data::font
