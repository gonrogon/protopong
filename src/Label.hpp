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

#include "Entity.hpp"
#include <glm/glm.hpp>
#include <string>

namespace pong {

/**
 * @brief An entity that displays text.
 *
 * This class handles text rendering, including alignment and color. It uses a "dirty flag" optimization to only
 * recalculate text geometry when the text content or its properties (alignment, width) actually change, significantly
 * improving performance for static labels.
 * @see pong::data::font::getGlyph
 */
class Label final : public Entity
{
    /**
     * @brief Stores pre-calculated position and size for a single quad used to render a character segment.
     */
    struct CharQuad
    {
        glm::vec2 position;
        glm::vec2 size;
    };

public:

    /**
     * @brief Define an enumeration with the horizontal alignment modes.
     */
    enum class HAlign { Left, Center, Right };

    /**
     * @brief Define an enumeration with the vertical alignment modes.
     */
    enum class VAlign { Top, Middle, Bottom };

    /**
     * @brief Constructor.
     * @param width Desired width scale of the text.
     * @param position Base position of the label.
     * @param color RGBA color of the text.
     * @param text Text to display.
     */
    Label(float width, glm::vec2 position, const glm::vec4& color, std::string text) noexcept;

    /**
     * @brief Gets the horizontal alignment.
     * @return Horizontal alignment.
     */
    [[nodiscard]] HAlign hAlign() const noexcept { return mHAlign; }

    /**
     * @brief Sets the horizontal alignment.
     * @param h Horizontal alignment to set.
     */
    void setHAlign(HAlign h);

    /**
     * @brief Gets the vertical alignment.
     * @return Vertical alignment.
     */
    [[nodiscard]] VAlign vAlign() const noexcept { return mVAlign; }

    /**
     * @brief Sets the vertical alignment.
     * @param v Vertical alignment to set.
     */
    void setVAlign(VAlign v);

    /**
     * @brief Sets the horizontal and vertical alignment.
     * @param h Horizontal alignment to set.
     * @param v Vertical alignment to set.
     */
    void setAlign(HAlign h, VAlign v);

    /**
     * @brief Gets the text.
     * @return Text.
     */
    [[nodiscard]] const std::string& text() const noexcept { return mText; }

    /**
     * @brief Sets the text.
     * @param text Text to set.
     */
    void setText(std::string text);

    /**
     * @brief Draws the label on the screen.
     *
     * If the label's geometry is dirty, it is recalculated before drawing.
     */
    void draw(Renderer& renderer, float interp) override;

private:

    /**
     * @brief Recalculates the geometry of the characters quads.
     *
     * This potentially costly method is called only when the text content or its rendering properties (alignment,
     * width) have changed (i.e., `mDirty` is true).
     */
    void updateGeometry();

private:

    /** @brief Width. */
    float mWidth = 1.0f;

    /** @brief Horizontal alignment. */
    HAlign mHAlign = HAlign::Center;

    /** @brief Vertical alignment. */
    VAlign mVAlign = VAlign::Middle;

    /** @brief Position. */
    glm::vec2 mPosition;

    /** @brief Color. */
    glm::vec4 mColor = glm::vec4(1.0f);

    /** @brief Text. */
    std::string mText = "label";

    /** @brief Flag indicating whether the geometry needs recalculation or not. */
    bool mDirty = true;

    /** @brief List of characters quads to render. */
    std::vector<CharQuad> mCharQuads;
};

} // namespace pong
