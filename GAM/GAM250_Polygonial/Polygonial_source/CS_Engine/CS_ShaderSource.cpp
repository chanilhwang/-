/**
 * @file CS_ShaderSource.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include <string>
#include "CS_ShaderSource.hpp"
namespace ShaderSource
{
    const std::string vertex_position =
        R"(
#version 330

layout(location = 0) in vec2 position;
uniform mat3 transform;
uniform float depth;

void main()
{
    vec3 position = transform * vec3(position, 1.0f);
    gl_Position = vec4(position.xy, depth, 1.0);
}
)";
    const std::string fragment_uniformcolor =
        R"(
#version 330

uniform vec4 color;
out vec4 output_color;

void main()
{
    output_color = color;
}
)";


    const std::string vertex_positioncolor =
        R"(
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

uniform mat3 transform;
uniform float depth;

smooth out vec4 interpolated_color;

void main()
{
    vec3 position = transform * vec3(position, 1.0f);
    gl_Position = vec4(position.xy, depth, 1.0);
    interpolated_color = color;
}
)";


    const std::string fragment_color =
        R"(
#version 330

in vec4 interpolated_color;
out vec4 output_color;

void main()
{
    output_color = interpolated_color;
}
)";


    const std::string vertex_positiontexture =
        R"(
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture_position;
uniform mat3 WtoCtoNDC;
uniform mat3 MtoW;
uniform float depth;

out vec2 interpolated_texture_position;

void main()
{
    //vec3 position = transform * vec3(position, 1.0f);
    vec3 position = WtoCtoNDC*MtoW*vec3(position, 1.0f);
    gl_Position = vec4(position.xy, depth, 1.0);
    interpolated_texture_position = texture_position;
}
)";
    ;
    const std::string fragment_uniformcolortexture =
        R"(
#version 330

in vec2 interpolated_texture_position;

uniform vec4 color;
uniform sampler2D texture_sampler;

out vec4 output_color;

void main()
{
    vec4 texel = texture(texture_sampler, interpolated_texture_position);
    vec4 new_color = color * texel;
    if(new_color.a <= 0.0f) // if the alpha value is zero then we should not write this pixel at all
        discard;
    output_color = new_color;
}
)";
}