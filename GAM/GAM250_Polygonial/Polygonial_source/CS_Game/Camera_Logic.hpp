/**
 * @file Camera_Logic.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Logics for camera movement
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include <glm/vec2.hpp>
class Player_Component;

class CS_Camera;
class CS_Object;
class Camera_Logic
{
public:
    Camera_Logic();
    void camera_tracing(float dt, CS_Camera* level_camera);
    void init(CS_Camera* level_camera);

private:
	glm::vec2 max_cam_pos_from_mouse,
		max_cam_pos_from_player,
		max_cam_pos_move,
		map_size;

	glm::vec2 cam_pos_from_mouse,
		cam_pos_from_player,
		cam_vel;

    CS_Object* player;
    Player_Component* player_component;
};
