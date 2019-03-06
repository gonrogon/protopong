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
/// @file   src/Label.hpp
/// @date   2015-11-02
/// @author Gonzalo González Romero
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////

#include "Entity.hpp"
#include <string>

////////////////////////////////////////////////////////////

namespace pong {

////////////////////////////////////////////////////////////
/// @brief Define an entity for labels.
///
/// Labels allow to draw texts using the characters data (@see pong::data::Char).
////////////////////////////////////////////////////////////
class Label  : public Entity
{
public:

    /**
     * @brief Define an enumeration with the horizontal alignment modes.
     */
    enum class HAlign
    {
        Left,   //!< Left.
        Center, //!< Center
        Right,  //!< Right.
    };

    /**
     * @brief Define an enumeration with the vertical alignment modes.
     */
    enum class VAlign
    {
        Top,    //!< The top of the text is aligned with the pencil.
        Middle, //!< The text is centered on the pencil.
        Bottom, //!< The bottom of the text is aligned with the pencil.
    };

    /**
     * @brief Constructor.
     *
     * @param width    Width.
     * @param position Position.
     * @param color    Color.
     * @param text     Text.
     */
    Label(float width, glm::vec2 position, const glm::vec4& color, std::string text);

    /**
     * @return Horizontal alignment.
     */
    HAlign hAlign() const;

    /**
     * @brief Set the horizontal alignment.
     *
     * @param h Alignment to set.
     */
    void setHAlign(HAlign h);

    /**
     * @return Vertical alignment.
     */
    VAlign vAlign() const;

    /**
     * @brief Set the vertical alignment.
     *
     * @param v Alignment to set.
     */
    void setVAlign(VAlign v);

    /**
     * @brief Set the horizontal and vertical alignment.
     *
     * @param h Horizontal alignment to set.
     * @param v Vertical alignment to set.
     */
    void setAlign(HAlign h, VAlign v);

    /**
     * @return Text.
     */
    const std::string& text() const;

    /**
     * @brief Set the text.
     *
     * @param text Text to set.
     */
    void setText(std::string text);

    void draw(float dt, float interp, Renderer& renderer) override;

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
};

////////////////////////////////////////////////////////////

} // namespace pong
