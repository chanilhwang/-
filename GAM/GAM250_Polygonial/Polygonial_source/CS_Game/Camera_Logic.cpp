/**
 * @file Camera_Logic.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Logics for camera movement
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Camera_Logic.hpp"
#include "CS_Graphics.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Object.hpp"
#include "CS_Camera.hpp"
#include "Player_Component.hpp"
#include "CS_Input.hpp"
//#include "CS_Input.hpp"

namespace {
	constexpr float CAM_VEL = 400.f;
	constexpr float CAM_REGION = 0.7f;
	constexpr float PLAYER_REGION = 0.4f;
	constexpr float MAP_SIZE = 2.5f;
	constexpr float MAX_MOVE = 0.2f;
}

Camera_Logic::Camera_Logic()
{
	max_cam_pos_from_mouse.x = CAM_REGION * CS_Graphics::GetDisplayAreaSize().x / 2.f;
	max_cam_pos_from_mouse.y = CAM_REGION * CS_Graphics::GetDisplayAreaSize().y / 2.f;

	max_cam_pos_from_player.x = PLAYER_REGION * CS_Graphics::GetDisplayAreaSize().x / 2.f;
	max_cam_pos_from_player.y = PLAYER_REGION * CS_Graphics::GetDisplayAreaSize().y / 2.f;

	map_size.x = MAP_SIZE * CS_Graphics::GetDisplayAreaSize().x / 2.f;
	map_size.y = MAP_SIZE * CS_Graphics::GetDisplayAreaSize().y / 2.f;

	max_cam_pos_move.x = MAX_MOVE * CS_Graphics::GetDisplayAreaSize().x / 2.f;
	max_cam_pos_move.y = MAX_MOVE * CS_Graphics::GetDisplayAreaSize().y / 2.f;
}
void Camera_Logic::init(CS_Camera* level_cam)
{
    player = CS_ObjectManager::GetFirstObjectByType(CS_ArcheTypes::Player);
    if (!player)
        return;
	player->GetComponent(CS_ComponentTypes::Player_Component, player_component);
	player_component->camera = this;

    level_cam->SetCenter(glm::vec3(0, 0, 0));

	cam_pos_from_player = glm::vec2(0, 0);
	cam_pos_from_mouse = glm::vec2(0, 0);
}

void Camera_Logic::camera_tracing(float dt, CS_Camera* level_cam)
{
    if(!player)
    {
		init(level_cam);
    }
    glm::vec2 player_pos = player->transform.GetTranslation();
	bool RIGHT = player_pos.x >= map_size.x,
		LEFT = player_pos.x <= -map_size.x,
		UP = player_pos.y >= map_size.y,
		DOWN = player_pos.y <= -map_size.y;
	if (RIGHT)
		player->transform.SetTranslation_x(map_size.x);
	else if (LEFT)
		player->transform.SetTranslation_x(-map_size.x);
	if(UP)
		player->transform.SetTranslation_y(map_size.y);
	else if(DOWN)
		player->transform.SetTranslation_y(-map_size.y);

	glm::vec2 dif = player_pos - cam_pos_from_player;
	RIGHT = dif.x >= max_cam_pos_from_player.x;
	LEFT = dif.x <= -max_cam_pos_from_player.x;
	UP = dif.y >= max_cam_pos_from_player.y;
	DOWN = dif.y <= -max_cam_pos_from_player.y;
	if (RIGHT)
		cam_pos_from_player.x += dif.x - max_cam_pos_from_player.x;
	else if(LEFT)
		cam_pos_from_player.x += dif.x + max_cam_pos_from_player.x;
	if (UP)
		cam_pos_from_player.y += dif.y - max_cam_pos_from_player.y;
	else if (DOWN)
		cam_pos_from_player.y += dif.y + max_cam_pos_from_player.y;

    glm::vec2 mouse_pos = CS_Input::GetMouseWorldPos();
	RIGHT = mouse_pos.x >= max_cam_pos_from_mouse.x;
	LEFT = mouse_pos.x <= -max_cam_pos_from_mouse.x;
	UP = mouse_pos.y >= max_cam_pos_from_mouse.y;
	DOWN = mouse_pos.y <= -max_cam_pos_from_mouse.y;
	if (cam_pos_from_mouse.x <= max_cam_pos_move.x && RIGHT)
		cam_vel.x = CAM_VEL;
	else if (cam_pos_from_mouse.x >= -max_cam_pos_move.x && LEFT)
		cam_vel.x = -CAM_VEL;
	else if (cam_pos_from_mouse.x >= -10 && cam_pos_from_mouse.x <= 10 || RIGHT || LEFT)
		cam_vel.x = 0;
	else
		cam_vel.x = (cam_pos_from_mouse.x >= 0 ? -1 : 1) * CAM_VEL;
	if (cam_pos_from_mouse.y <= max_cam_pos_move.y && UP)
		cam_vel.y = CAM_VEL;
	else if (cam_pos_from_mouse.y >= -max_cam_pos_move.y && DOWN)
		cam_vel.y = -CAM_VEL;
	else if (cam_pos_from_mouse.y >= -10 && cam_pos_from_mouse.y <= 10 || UP || DOWN)
		cam_vel.y = 0;
	else
		cam_vel.y = (cam_pos_from_mouse.y >= 0 ? -1 : 1) * CAM_VEL;

	cam_pos_from_mouse += cam_vel * dt;
    level_cam->SetCenter(cam_pos_from_mouse + cam_pos_from_player);
}