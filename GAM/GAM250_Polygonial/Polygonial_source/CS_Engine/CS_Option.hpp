/**
 * @file CS_Option.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief header file for option level
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once

#include "glm/glm.hpp"

class CS_Options {
public:
	static float GetVolume();
	static void SetVolume(float volume);
	static bool IsFullScreen();
	static void ChangeToFullScreen(bool IsFullScreen);
	static void ChangeResolution(int width, int height);
	static glm::vec2 GetResolution();

	static void Init(glm::vec2 resolution,
		bool isfullscreen,
		float volume);
};