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
/// @file   src/Label.cpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#include "Label.hpp"
#include "data/Char.hpp"
#include "Renderer.hpp"
#include <utf8d/utf8d.hpp>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////

Label::Label(const float width, const glm::vec2 position, const glm::vec4& color, std::string text)
    :
    Entity   (Type::Label),
    mWidth   (width),
    mPosition(position),
    mColor   (color),
    mText    (std::move(text))
{}

////////////////////////////////////////////////////////////

void Label::setAlign(const HAlign h, const VAlign v)
{
    setHAlign(h);
    setVAlign(v);
}

////////////////////////////////////////////////////////////

void Label::draw(const float dt, const float interp, Renderer& renderer)
{
    int   n      = 0;
    float ox     = 0;
    float oy     = 0;
    float scale  = mWidth / data::Char::Advance;
    // Horizontal alignment.
    switch (mHAlign)
    {
        case HAlign::Left:   { ox = 0.0f; } break;
        case HAlign::Center: { ox = static_cast<float>(mText.length()) * -mWidth * 0.5f; } break;
        case HAlign::Right:  { ox = static_cast<float>(mText.length()) * -mWidth; } break;
    }
    // Vertical alignment.
    switch (mVAlign)
    {
        case VAlign::Top:    { oy -= static_cast<float>(data::Char::MaxHeight) * scale; } break;
        case VAlign::Middle: { oy -= static_cast<float>(data::Char::MaxHeight) * scale * 0.5f; } break;
        case VAlign::Bottom: { oy  = 0.0f; } break;
    }
    // Decode the string to print the label.
    for (utf8d::UInt32 cpos = 0, state = 0, codepoint; cpos < mText.length(); ++cpos)
    {
        if (utf8d::decode(static_cast<utf8d::UInt08 >(mText[cpos]), state, codepoint) == utf8d::Accept)
        {
            const unsigned char* cd;

            if ((codepoint != 0x20) && ((cd = data::Char::glyph(codepoint)) != nullptr))
            {
                float adv = ox + mWidth * static_cast<float>(n);

                for (int i = 2; i <= cd[0] * 4 + 1; i += 4)
                {
                    glm::vec2 pos(static_cast<float>(cd[i]),     static_cast<float>(cd[i + 1]));
                    glm::vec2 sca(static_cast<float>(cd[i + 2]), static_cast<float>(cd[i + 3]));
                    // Center the character in its bounding box.
                    pos.x += static_cast<float>(data::Char::Advance - cd[1]) * 0.5f;
                    // Scale position.
                    pos.x = pos.x * scale + sca.x * scale * 0.5f + adv;
                    pos.y = pos.y * scale + sca.y * scale * 0.5f + oy;
                    // Scale size.
                    sca *= scale;
                    // Draw.
                    renderer.queueQuad(pos + mPosition, sca, mColor);
                }
            }

            n++;
        }
    }
}

////////////////////////////////////////////////////////////

} // namespace pong
