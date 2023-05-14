/**
 * @file CS_Input.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief List of receivable button inputs.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Input.hpp"
#include <bitset>
#include <stack>
#include <iostream>
#include "CS_Graphics.hpp"
#include "CS_Camera.hpp"
namespace InputInfo
{
    std::bitset<static_cast<size_t>(InputButtons::Count)>  s_pressed;/*!< Array of keys pressed this frame*/
    std::bitset<static_cast<size_t>(InputButtons::Count)>  s_released;/*!< Array of keys released this frame*/
    std::bitset<static_cast<size_t>(InputButtons::Count)>  s_triggered;/*!< Array of keys triggered this frame*/
    std::bitset<static_cast<size_t>(InputButtons::Count)>  s_repeating;/*!< Array of keys triggered this frame*/
    std::stack<InputButtons> s_unpressed;            /*!< The array of keys to unpress*/
    float s_scrollAmount;
    glm::vec3 mouseWorldPos{};
	glm::vec2 mouseCamPos{};
	glm::vec3 mouseHUDPos{};
}


bool CS_Input::IsPressed(InputButtons key)
{
	return InputInfo::s_pressed[static_cast<int>(key)];
}

bool CS_Input::IsReleased(InputButtons key)
{
    return InputInfo::s_released[static_cast<int>(key)];
}

bool CS_Input::IsTriggered(InputButtons key)
{
	return InputInfo::s_triggered[static_cast<int>(key)];
}

bool CS_Input::IsRepeating(InputButtons key)
{
	return InputInfo::s_repeating[static_cast<int>(key)];
}

bool CS_Input::IsAnyPressed(void)
{
	return InputInfo::s_pressed.any();
}

bool CS_Input::IsAnyReleased(void)
{
    return InputInfo::s_released.any();
}

bool CS_Input::IsAnyTriggered(void)
{
	return InputInfo::s_triggered.any();
}

bool CS_Input::IsAnyRepeating(void)
{
	return InputInfo::s_repeating.any();
}

glm::vec2 CS_Input::GetMouseWorldPos(void)
{
	return glm::vec2{ InputInfo::mouseWorldPos.x,InputInfo::mouseWorldPos.y };
}

glm::vec2 CS_Input::GetMouseCameraPos(void)
{
	return InputInfo::mouseCamPos;
}

glm::vec2 CS_Input::GetMouseHUDPos(void)
{
	return InputInfo::mouseHUDPos;
}

float CS_Input::GetMouseWheelScrollAmount()
{
    return InputInfo::s_scrollAmount;
}

void CS_Input::Init(void)
{
	for (int i = 0; i < static_cast<int>(InputButtons::Count); ++i)
	{
		InputInfo::s_pressed[i] = false;
		InputInfo::s_triggered[i] = false;
		InputInfo::s_repeating[i] = false;
	}
	InputInfo::s_scrollAmount = 0.f;
	InputInfo::mouseCamPos.x = 0.f;
	InputInfo::mouseCamPos.y = 0.f;
	InputInfo::mouseWorldPos.x = 0.f;
	InputInfo::mouseWorldPos.y = 0.f;
	InputInfo::mouseHUDPos.x = 0.f;
	InputInfo::mouseHUDPos.y = 0.f;
}

void CS_Input::Rest(void)
{
	/*Only reset the pressed values that are not being pressed*/
	for (size_t i = 0; i < InputInfo::s_unpressed.size(); ++i)
	{
		InputInfo::s_pressed[static_cast<int>(InputInfo::s_unpressed.top())] = false;/*Reset the pressed value*/
		InputInfo::s_unpressed.pop();
	}
	for (int i = 0; i < static_cast<int>(InputButtons::Count); ++i)
	{
		InputInfo::s_triggered[i] = false;
		InputInfo::s_repeating[i] = false;
                InputInfo::s_released[i] = false;
	}
	InputInfo::s_scrollAmount = 0.f;
}

void CS_Input::SetPressed(InputButtons key, bool state)
{
	if(key==InputButtons::Count||key==InputButtons::None)
		return;
	if(state)
	{
		if(InputInfo::s_pressed[static_cast<int>(key)])
		{
			InputInfo::s_repeating[static_cast<int>(key)] = true;
		}
		else
		{
			InputInfo::s_pressed[static_cast<int>(key)] = true;
			InputInfo::s_triggered[static_cast<int>(key)] = true;
		}
	}
	else
	{
		InputInfo::s_unpressed.push(key);
                InputInfo::s_released[static_cast<int>(key)] = true;
	}
}


void CS_Input::SetMouse(int x, int y)
{
	CS_Camera& currentCam = CS_Graphics::GetViewCamera();
	CS_Camera& HUDCam = CS_Graphics::GetHUDCamera();
	const glm::vec2 areaSize = CS_Graphics::GetDisplayAreaSize();
	const float inverseZoom = 1.f / currentCam.GetZoom();
	InputInfo::mouseCamPos = { x - areaSize.x / 2.f,-y + areaSize.y / 2.f };
	InputInfo::mouseCamPos *= inverseZoom;
	InputInfo::mouseWorldPos =currentCam.CameraToWorld()
	*glm::vec3{ InputInfo::mouseCamPos.x,InputInfo::mouseCamPos.y,1.f };
	InputInfo::mouseHUDPos = HUDCam.CameraToWorld()*glm::vec3{ InputInfo::mouseCamPos.x,InputInfo::mouseCamPos.y,1.f };
}

void CS_Input::SetMouseWheelScrollAmount(int amount)
{
	InputInfo::s_scrollAmount = float(amount);
}


