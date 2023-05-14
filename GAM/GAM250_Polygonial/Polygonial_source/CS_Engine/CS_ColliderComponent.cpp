/**
 * @file CS_ColliderComponent.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For collision detection.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_ColliderComponent.hpp"
#include <iostream>
bool CS_IntersectCheck::Circle_Circle(const glm::vec2 & circleCenter0, float radius0, const glm::vec2 & circleCenter1, float radius1)
{
    return glm::distance(circleCenter0, circleCenter1) <= (radius0 + radius1);
}

bool CS_IntersectCheck::AABB_Box_Circle(const glm::vec2& Right_Top, const glm::vec2& Left_Bot,
    const glm::vec2& circleCenter, float radius)
{
    glm::vec2 ClosestPoint = circleCenter;
    ClosestPoint.x = (ClosestPoint.x < Left_Bot.x) ? Left_Bot.x : ClosestPoint.x;
    ClosestPoint.x = (ClosestPoint.x > Right_Top.x) ? Right_Top.x : ClosestPoint.x;

    ClosestPoint.y = (ClosestPoint.y < Left_Bot.y) ? Left_Bot.y : ClosestPoint.y;
    ClosestPoint.y = (ClosestPoint.y > Right_Top.y) ? Right_Top.y : ClosestPoint.y;

    return glm::distance(circleCenter, ClosestPoint) <= radius;
}

bool CS_IntersectCheck::AABB_Box_AABB_Box(const glm::vec2& Right_Top1, const glm::vec2& Left_Bot1,
    const glm::vec2& Right_Top2, const glm::vec2& Left_Bot2)
{
    const bool overX = ((Left_Bot2.x <= Right_Top1.x) && (Left_Bot1.x <= Right_Top2.x));
    const bool overY = ((Left_Bot2.y <= Right_Top1.y) && (Left_Bot1.y <= Right_Top2.y));

    return overX && overY;
}


bool CS_IntersectCheck::OBB_Box_OBB_Box(const glm::vec2 center1, const glm::vec2 Up1, const glm::vec2 Right1,
    const glm::vec2 half_lengths1, const glm::vec2 center2, const glm::vec2 Up2, const glm::vec2 Right2,
    const glm::vec2 half_lengths2)
{
    glm::vec2 direction = center2 - center1;
    glm::vec2 line[8] = { Up1, Right1, Up2, Right2, -Up1, -Right1, -Up2, -Right2 };
    for(int i=0; i<8; i++)
    {
        if(abs(glm::dot(direction, line[i]) > abs(glm::dot(half_lengths1.x*Right1, line[i]))
            + abs(glm::dot(half_lengths1.y*Up1, line[i]))
            + abs(glm::dot(half_lengths2.x*Right2, line[i]))
            + abs(glm::dot(half_lengths2.y*Up2, line[i]))))
        {
            return false;
        }
    }
    return true;
}

bool CS_IntersectCheck::OBB_Box_AABB_Box(const glm::vec2& Right_Top1, const glm::vec2& Left_Bot1,
    const glm::vec2& Right_Top2, const glm::vec2& Left_Bot2)
{
    const bool overX = ((Left_Bot2.x <= Right_Top1.x) && (Left_Bot1.x <= Right_Top2.x));
    const bool overY = ((Left_Bot2.y <= Right_Top1.y) && (Left_Bot1.y <= Right_Top2.y));
   
    return overX && overY;
}

bool CS_IntersectCheck::OBB_Box_Circle(const glm::vec2& Right_Top, const glm::vec2& Left_Bot,
    const glm::vec2& circleCenter, float radius)
{
    glm::vec2 ClosestPoint = circleCenter;
    ClosestPoint.x = (ClosestPoint.x < Left_Bot.x) ? Left_Bot.x : ClosestPoint.x;
    ClosestPoint.x = (ClosestPoint.x > Right_Top.x) ? Right_Top.x : ClosestPoint.x;

    ClosestPoint.y = (ClosestPoint.y < Left_Bot.y) ? Left_Bot.y : ClosestPoint.y;
    ClosestPoint.y = (ClosestPoint.y > Right_Top.y) ? Right_Top.y : ClosestPoint.y;
  
    return glm::distance(circleCenter, ClosestPoint) <= radius;
}
