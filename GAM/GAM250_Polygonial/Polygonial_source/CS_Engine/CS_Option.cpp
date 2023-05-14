/**
 * @file CS_Option.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief implementation file for option level
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Option.hpp"
#include "CS_Application.hpp"
#include "CS_SoundManager.hpp"

namespace {
	int resolution[2];
	bool isfullscreen;
	float volume;
}

float CS_Options::GetVolume() {
	return volume;
}
void CS_Options::SetVolume(float new_volume) {
	volume = new_volume;
	CS_SoundManager::SetMasterVolume(volume);
}
bool CS_Options::IsFullScreen() {
	return isfullscreen;
}
void CS_Options::ChangeToFullScreen(bool IsFullScreen) {
	isfullscreen = IsFullScreen;
	CS_Application::ChangeToFullScreen(IsFullScreen);
}
void CS_Options::ChangeResolution(int width, int height) {
	CS_Application::Resize(width, height);
	resolution[0] = width;
	resolution[1] = height;
}
glm::vec2 CS_Options::GetResolution() {
	return glm::vec2(resolution[0], resolution[1]);
}
void CS_Options::Init(glm::vec2 new_resolution,
	bool new_isfullscreen,
	float new_volume) {
	resolution[0] = static_cast<int>(new_resolution.x);
	resolution[1] = static_cast<int>(new_resolution.y);
	isfullscreen = new_isfullscreen;
	volume = new_volume;
}