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

#include "Label.hpp"
#include "Renderer.hpp"
#include "data/Char.hpp"
#include <utf8d/utf8d.hpp>

namespace pong {

Label::Label(const float width, const glm::vec2 position, const glm::vec4& color, std::string text) noexcept
    :
    Entity(Type::Label), mWidth(width), mPosition(position), mColor(color), mText(std::move(text))
{}

void Label::setHAlign(const HAlign h)
{
    mHAlign = h;
    mDirty  = true;
}

void Label::setVAlign(const VAlign v)
{
    mVAlign = v;
    mDirty  = true;
}

void Label::setAlign(const HAlign h, const VAlign v)
{
    setHAlign(h);
    setVAlign(v);
}

void Label::setText(std::string text)
{
    mText  = std::move(text);
    mDirty = true;
}

void Label::draw(Renderer& renderer, const float interp)
{
    if (mDirty)
    {
        mDirty = false;
        updateGeometry();
    }

    for (const auto& [position, size] : mCharQuads)
    {
        renderer.queueQuad(position, size, mColor);
    }
}

void Label::updateGeometry()
{
    mCharQuads.clear();
    if (mText.empty())
    {
        return;
    }

    float ox = 0;
    float oy = 0;
    const float scale  = mWidth / data::font::Advance;
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
        case VAlign::Top:    { oy -= static_cast<float>(data::font::MaxHeight) * scale; } break;
        case VAlign::Middle: { oy -= static_cast<float>(data::font::MaxHeight) * scale * 0.5f; } break;
        case VAlign::Bottom: { oy  = 0.0f; } break;
    }

    int n = 0;
    // Decode the string and generate the quads to render.
    for (uint32_t cpos = 0, state = 0, codepoint; cpos < mText.length(); ++cpos)
    {
        if (utf8d::decode(static_cast<uint8_t>(mText[cpos]), state, codepoint) == utf8d::Accept)
        {
            if (codepoint != 0x20) {
                if (auto span = data::font::getGlyph(codepoint); !span.empty()) {
                    const float advance = ox + mWidth * static_cast<float>(n);

                    for (int i = 2; i < span.size(); i += 4)
                    {
                        glm::vec2 pos(static_cast<float>(span[i]),     static_cast<float>(span[i + 1]));
                        glm::vec2 sca(static_cast<float>(span[i + 2]), static_cast<float>(span[i + 3]));
                        // Center the character in its bounding box.
                        pos.x += static_cast<float>(data::font::Advance - span[1]) * 0.5f;
                        // Scale position.
                        pos.x = pos.x * scale + sca.x * scale * 0.5f + advance;
                        pos.y = pos.y * scale + sca.y * scale * 0.5f + oy;
                        // Scale size.
                        sca *= scale;

                        mCharQuads.emplace_back(pos + mPosition, sca);
                    }
                }
            }

            n++;
        }
    }
}

} // namespace pong
