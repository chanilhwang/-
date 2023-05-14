/******************************************************************************/
/*!
\file   Application.h
\author David Ly
\par	dly
\par    email: dly\@digipen.edu
\par    CS230
\par    Game State Manager
\date   2017/05/17

This is the Application layer for creating and controlling a window for the
CS230

*/
/******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

/*! Used to exclude rarely-used stuff from Windows since we don't use them */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct InitData {
	HINSTANCE   hInstance;		/* The hInstance from WinMain */
	const char* title;			/* The title of the window */
	bool		fullscreen;		/* Fullscreen boolean */
	bool		quit;			/* Quit application boolean */
};

/* A singleton class for application window */
class Application {
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
public:
	static void InitWindowClass(const InitData& initData);
	static void ProcessMessages(void);
	static void Update(void);
	
};


#endif