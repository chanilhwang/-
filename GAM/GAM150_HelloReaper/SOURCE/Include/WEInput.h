/******************************************************************************/
/*!
\file   WEInput.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

Prototypes for input funtions

*/
/******************************************************************************/
#ifndef WE_INPUT_H
#define WE_INPUT_H

//Forward Declarations
struct WEVec2;

//! The WarpEngine interal set of keys/buttons that can be pressed.
enum WEKeyCode
{
  WE_INVALID,
  WE_BACKSPACE,
  WE_TAB,
  WE_RETURN,
  WE_SHIFT,
  WE_CONTROL,
  WE_CAPS_LOCK,
  WE_ESCAPE,
  WE_SPACE,
  WE_ARROW_UP,
  WE_ARROW_DOWN,
  WE_ARROW_LEFT,
  WE_ARROW_RIGHT,
  WE_0,
  WE_1,
  WE_2,
  WE_3,
  WE_4,
  WE_5,
  WE_6,
  WE_7,
  WE_8,
  WE_9,
  WE_A,
  WE_B,
  WE_C,
  WE_D,
  WE_E,
  WE_F,
  WE_G,
  WE_H,
  WE_I,
  WE_J,
  WE_K,
  WE_L,
  WE_M,
  WE_N,
  WE_O,
  WE_P,
  WE_Q,
  WE_R,
  WE_S,
  WE_T,
  WE_U,
  WE_V,
  WE_W,
  WE_X,
  WE_Y,
  WE_Z,
  WE_F1,
  WE_F2,
  WE_F3,
  WE_F4,
  WE_F5,
  WE_F6,
  WE_F7,
  WE_F8,
  WE_F9,
  WE_F10,
  WE_F11,
  WE_F12,
  WE_SHIFT_LEFT,
  WE_SHIFT_RIGHT,
  WE_CONTROL_LEFT,
  WE_CONTROL_RIGHT,
  WE_MOUSE_LEFT,
  WE_MOUSE_RIGHT,
  WE_MOUSE_MIDDLE,
  WE_MOUSE_MIDDLE_UP,
  WE_MOUSE_MIDDLE_DOWN,
  WE_GAMEPAD_DPAD_UP,
  WE_GAMEPAD_DPAD_DOWN,
  WE_GAMEPAD_DPAD_LEFT,
  WE_GAMEPAD_DPAD_RIGHT,
  WE_GAMEPAD_START,
  WE_GAMEPAD_BACK,
  WE_GAMEPAD_LEFT_THUMB,
  WE_GAMEPAD_RIGHT_THUMB,
  WE_GAMEPAD_LEFT_SHOULDER,
  WE_GAMEPAD_RIGHT_SHOULDER,
  WE_GAMEPAD_A,
  WE_GAMEPAD_B,
  WE_GAMEPAD_X,
  WE_GAMEPAD_Y,
  WE_LAST
};

namespace WEInput
{
 /*Check if a specific key is being held down*/
bool  IsPressed(WEKeyCode key);
/*Check if a specific key was first touched this frame*/
bool  IsTriggered(WEKeyCode key);
/*Check if key is touched this frame and last frame*/
bool  IsRepeating(WEKeyCode key);
/*Check if any key is being held down*/
bool  IsAnyPressed(void);
/*Check if any key was first touched this frame*/
bool  IsAnyTriggered(void);
/*Check if any key is touched this frame and last*/
bool  IsAnyRepeating(void);
/*Gets the mouse click point in screen space*/
void  GetMouse(WEVec2& pMouse);
/*Gets the direction of the left thumbstick*/
void  GetLeftThumb(WEVec2& vec);
/*Gets the direction of the right thumbstick*/
void  GetRightThumb(WEVec2& vec);
/*Gets the value of the left trigger*/
float GetLeftTrigger(void);
/*Gets the value of the right trigger*/
float GetRightTrigger(void);
/*Lets user know if game pad is connected*/
bool  GamePadIsConnected(void);
}//end namespace WEInput




#endif