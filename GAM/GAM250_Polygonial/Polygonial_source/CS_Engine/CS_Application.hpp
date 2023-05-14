#pragma once
#include "CS_WinHeader.hpp"
#include "CS_GameData.hpp"
#include <glm/vec2.hpp>
class CS_Graphics;
class CS_StageManager;
struct CS_InitData
{
    std::string Title="";
    std::string StartStage ="";
    int Height=1080;  
    int Width= 1920;
	bool IsFullScreen = true;
	float volume = 1.0f;
    CS_GameData GameData;
};

class CS_Application
{
    friend CS_Graphics;
	friend CS_StageManager;
public:
    static bool Init(CS_InitData& initData);
    static void Update();
    static void ShutDown();
    static void EndAll();
    static glm::vec2 GetWindowSize();
	static void Resize(int new_width, int new_height);
	static void ChangeToFullScreen(bool IsFullScreen);
private:
    static PIXELFORMATDESCRIPTOR MakePixelDescription();
    static bool MakeOldOpenglWindow();
    static void RegisterOpenglClass();
    static bool MakeOpenGLContext();
    static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void PeekMessages();
    static void SwapBuffer();
    


};

