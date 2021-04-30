/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, nect
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef VT100_H
#define VT100_H

#include <iostream>

namespace vt100 {

    enum class Color4Bit
    {
        Black = 0,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        BrightBlack,
        BrightRed,
        BrightGreen,
        BrightYellow,
        BrightBlue,
        BrightMagenta,
        BrightCyan,
        BrightWhite
    };

    namespace internal {

        enum ColorType {
            BIT4,
            BIT8,
            BIT16
        };

        class Color
        {
        public:
            Color(Color4Bit color) : m_Type(BIT4)
            {
                m_Value.bit4 = color;
            }

            Color(uint8_t color) : m_Type(BIT8)
            {
                m_Value.bit8 = color;
            }

            Color(uint8_t r, uint8_t g, uint8_t b) : m_Type(BIT16)
            {
                m_Value.bit16.r = r;
                m_Value.bit16.g = g;
                m_Value.bit16.b = b;
            }

        protected:
            union {
                Color4Bit bit4;
                uint8_t bit8;
                struct {
                    uint8_t r;
                    uint8_t g;
                    uint8_t b;
                } bit16;
            } m_Value;
            ColorType m_Type;
        };

        struct Attribute
        {
            Attribute(uint8_t code) : m_Code(code)
            {}

        protected:
            uint8_t m_Code;
            friend std::ostream& operator<<(std::ostream& os, const Attribute& attribute);
        };

        std::ostream& operator<<(std::ostream& os, const Attribute& attribute)
        {
            os << "\x1b[" << attribute.m_Code << "m";
            return os;
        }

    }

    class Fg : public internal::Color
    {
        using internal::Color::Color;
        friend std::ostream& operator<<(std::ostream& os, const Fg& fg);
    };

    std::ostream& operator<<(std::ostream& os, const Fg& fg)
    {
        switch (fg.m_Type)
        {
            case internal::BIT4:
                if (fg.m_Value.bit4 > Color4Bit::White)
                    os << "\x1b[" << (static_cast<int>(fg.m_Value.bit4) + 90) << "m";
                else
                    os << "\x1b[" << (static_cast<int>(fg.m_Value.bit4) + 30) << "m";
                break;

            case internal::BIT8:
                os << "\x1b[38;5;" << fg.m_Value.bit8 << "m";
                break;

            case internal::BIT16:
                os << "\x1b[38;2;";
                os << fg.m_Value.bit16.r << ";";
                os << fg.m_Value.bit16.g << ";";
                os << fg.m_Value.bit16.b << "m";
                break;
        }

        return os;
    }

    class Bg : public internal::Color
    {
        using Color::Color;
        friend std::ostream& operator<<(std::ostream& os, const Bg& bg);
    };

    std::ostream& operator<<(std::ostream& os, const Bg& bg)
    {
        switch (bg.m_Type)
        {
            case internal::BIT4:
                if (bg.m_Value.bit4 > Color4Bit::White)
                    os << "\x1b[" << (static_cast<int>(bg.m_Value.bit4) + 92) << "m";
                else
                    os << "\x1b[" << (static_cast<int>(bg.m_Value.bit4) + 40) << "m";
                break;

            case internal::BIT8:
                os << "\x1b[48;5;" << bg.m_Value.bit8 << "m";
                break;

            case internal::BIT16:
                os << "\x1b[48;2;";
                os << bg.m_Value.bit16.r << ";";
                os << bg.m_Value.bit16.g << ";";
                os << bg.m_Value.bit16.b << "m";
                break;
        }

        return os;
    }

    auto Reset = internal::Attribute(0);
    auto Bold = internal::Attribute(1);
    auto Dim = internal::Attribute(2);
    auto Italic = internal::Attribute(3);
    auto Underline = internal::Attribute(4);
    auto Blink = internal::Attribute(5);
    auto Reverse = internal::Attribute(7);
    auto Hide = internal::Attribute(8);
    auto Strike = internal::Attribute(9);

    auto Primary = internal::Attribute(10);
    auto Alternative1 = internal::Attribute(11);
    auto Alternative2 = internal::Attribute(12);
    auto Alternative3 = internal::Attribute(13);
    auto Alternative4 = internal::Attribute(14);
    auto Alternative5 = internal::Attribute(15);
    auto Alternative6 = internal::Attribute(16);
    auto Alternative7 = internal::Attribute(17);
    auto Alternative8 = internal::Attribute(18);
    auto Alternative9 = internal::Attribute(19);

    auto Blackletter = internal::Attribute(20);
    auto DoubleUnderline = internal::Attribute(21);

    auto ResetIntensity = internal::Attribute(22);
    auto ResetItalic = internal::Attribute(23);
    auto ResetUnderline = internal::Attribute(24);
    auto ResetBlink = internal::Attribute(25);
    auto ResetReverse = internal::Attribute(27);
    auto ResetHide = internal::Attribute(28);
    auto ResetStrike = internal::Attribute(29);

}

#endif
