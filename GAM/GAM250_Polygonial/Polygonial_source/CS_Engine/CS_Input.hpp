/**
 * @file CS_Input.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief List of receivable button inputs.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <glm/vec2.hpp>
enum class InputButtons
{
	None,
	Mouse_Left,
	Mouse_Middle,
	Mouse_Right,
	Mouse_MiddleUp,
	Mouse_MiddleDown,
	Escape,
	Space,
	Left,
	Up,
	Right,
	Down,
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	NumPad_0,
	NumPad_1,
	NumPad_2,
	NumPad_3,
	NumPad_4,
	NumPad_5,
	NumPad_6,
	NumPad_7,
	NumPad_8,
	NumPad_9,
	F_5,
	Ctrl,
	Count
};
class CS_Application;
class CS_StageManager;
class CS_Input
{
	friend CS_Application;
	friend CS_StageManager;
public:
	/*Check if a specific key is being held down*/
	static bool  IsPressed(InputButtons key);
        static bool  IsReleased(InputButtons key);
	/*Check if a specific key was first touched this frame*/
	static bool  IsTriggered(InputButtons key);
	/*Check if key is touched this frame and last frame*/
	static bool  IsRepeating(InputButtons key);
	/*Check if any key is being held down*/
	static bool  IsAnyPressed(void);
        static bool  IsAnyReleased(void);
	/*Check if any key was first touched this frame*/
	static bool  IsAnyTriggered(void);
	/*Check if any key is touched this frame and last*/
	static bool  IsAnyRepeating(void);
	static glm::vec2 GetMouseWorldPos(void);
	static glm::vec2 GetMouseCameraPos(void);
	static glm::vec2 GetMouseHUDPos(void);
        static float GetMouseWheelScrollAmount();
		static void Rest(void);
private:
	static void Init(void);
	
	static void SetPressed(InputButtons key, bool state);
	static void SetMouse(int x, int y);
	static void SetMouseWheelScrollAmount(int amount);
        
};

