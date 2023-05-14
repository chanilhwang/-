/**
 * @file CS_Color.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For color management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "glm/vec4.hpp"

struct CS_Color
{
public:
    using ColorChannel = unsigned char;
    using RGBA32 = unsigned int;

    ColorChannel Red = 0;
    ColorChannel Green = 0;
    ColorChannel Blue = 0;
    ColorChannel Alpha = 255;

public:
    CS_Color() = default;
    CS_Color(ColorChannel red, ColorChannel green, ColorChannel blue, ColorChannel alpha = 255);
    explicit CS_Color(RGBA32 color);

    glm::vec4 ToRGBAfloat() const;
};

static_assert(sizeof(CS_Color) == sizeof(CS_Color::RGBA32),
    "Color should be the same as 4 bytes, so we can easily treat it as an RGBA int.");

namespace Colors
{
    extern const CS_Color Black;
    extern const CS_Color White;
    extern const CS_Color Red;
    extern const CS_Color Green;
    extern const CS_Color Blue;
    extern const CS_Color Yellow;
    extern const CS_Color Magenta;
    extern const CS_Color Cyan;
    extern const CS_Color Transparent;
}

