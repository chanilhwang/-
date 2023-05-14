/**
 * @file CS_Color.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For color management.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Color.hpp"



CS_Color::CS_Color(ColorChannel red, ColorChannel green, ColorChannel blue, ColorChannel alpha)
    : Red(red), Green(green), Blue(blue), Alpha(alpha)
{
}

CS_Color::CS_Color(RGBA32 color)
    : Red(static_cast<ColorChannel>((color & 0xff000000) >> 24)),
    Green(static_cast<ColorChannel>((color & 0x00ff0000) >> 16)),
    Blue(static_cast<ColorChannel>((color & 0x0000ff00) >> 8)),
    Alpha(static_cast<ColorChannel>((color & 0x000000ff) >> 0))
{
}

glm::vec4 CS_Color::ToRGBAfloat() const { return { Red / 255.0f, Green / 255.0f, Blue / 255.0f, Alpha / 255.0f }; }

namespace Colors
{
    const CS_Color Black(0, 0, 0);
    const CS_Color White(255, 255, 255);
    const CS_Color Red(255, 0, 0);
    const CS_Color Green(0, 255, 0);
    const CS_Color Blue(0, 0, 255);
    const CS_Color Yellow(255, 255, 0);
    const CS_Color Magenta(255, 0, 255);
    const CS_Color Cyan(0, 255, 255);
    const CS_Color Transparent(0, 0, 0, 0);
}
