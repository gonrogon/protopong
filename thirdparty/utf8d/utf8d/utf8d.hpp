////////////////////////////////////////////////////////////
/// UTF-8 Decoder (UTF8D)
////////////////////////////////////////////////////////////
/// UTF-8 Decoder is based on the Flexible and Economical UTF-8 Decoder by Bjoern
/// Hoehrmann <bjoern@hoehrmann.de> (http://bjoern.hoehrmann.de/utf-8/decoder/dfa),
/// it can be distributed and/or modified under the terms of the MIT License.
///
///=========================================================
/// The MIT License
///---------------------------------------------------------
/// Copyright (c) 2015 - 2025 Gonzalo González Romero (gonrogon)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
///=========================================================
/// Flexible and Economical UTF-8 Decoder
///---------------------------------------------------------
/// Flexible and Economical UTF-8 Decoder can be distributed and/or modified under
/// the terms of the MIT License.
///
///=========================================================
/// The MIT License
///---------------------------------------------------------
/// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
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

namespace utf8d {

inline constexpr uint32_t Accept =  0; //!< Accept code.
inline constexpr uint32_t Reject = 12; //!< Reject code.

/**
 * @brief Runs a decoding step.
 * 
 * A codepoint have been decoded when the function returns the accept code (@see uft::Accept).
 * Example:
 * @code
 *   std::string text = "example text";
 * 
 *   for (utf::UInt32 pos = 0, state = 0, codepoint; text.length() >= pos; ++pos)
 *   {
 *       if (decode(text[pos], state, codepoint) == utf::Accept)
 *       {
 *           // Process the codepoint
 *       }
 *   }
 * @endcode
 * 
 * @param byte Byte to process.
 * @param state Current state.
 * @param codepoint Codepoint.
 * 
 * @return State achieved after processing the byte.
 */
uint32_t decode(uint8_t byte, uint32_t& state, uint32_t& codepoint);

} // namespace utf
