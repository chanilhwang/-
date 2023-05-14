/**
 * @file CS_Math.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Transform building process.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Math.hpp"
namespace affine2D
{
	glm::mat3 build_translation(const glm::vec2 & translation)
	{
		glm::mat3 res(1.0f);
		res[2].x = translation.x;
		res[2].y = translation.y;
		return res;
	}
	glm::mat3 build_rotation(float rad)
	{
		glm::mat3 res(1.0f);
		auto cos = glm::cos(rad);
		auto sin = glm::sin(rad);
		res[0].x = cos;
		res[0].y = sin;
		res[1].x = -sin;
		res[1].y = cos;
		return res;
	}
	glm::mat3 build_scale(const glm::vec2 & scale)
	{
		glm::mat3 res(1.0f);
		res[0].x = scale.x;
		res[1].y = scale.y;
		return res;
	}
}

namespace mat2x2
{
    glm::mat2 build_rotation(float rad)
    {
        glm::mat2 res(1.0f);
        auto cos = glm::cos(rad);
        auto sin = glm::sin(rad);
        res[0].x = cos;
        res[0].y = sin;
        res[1].x = -sin;
        res[1].y = cos;
        return res;
    }
}