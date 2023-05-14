/******************************************************************************/
/*!
\file   Application.h
\author Your name
\par    email: Your email (example: dly\@digipen.edu)
\par    CS230
\par    Simple Windows Creation mA2
\date   Today's date

This is the Application layer for creating and controlling a window for the
CS230

*/
/******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

/*! Used to exclude rarely-used stuff from Windows since we don't use them */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*	Create a struct for InitData 
	I needs to contain 2 variables.
	Reference main.cpp to figure out what they are.
*/
struct InitData { /* Name the struct */
	/* The hInstance from WinMain */
	HINSTANCE hInstance;
	/* The title of the window */
	const char *title;
};

/* A singleton class for application window */
class Application {
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// Students may add code here. DO NOT change the public section.

public:
	static void InitWindowClass(const InitData& initData);
	static void ProcessMessages(MSG& msg);
	
};


#endif /* APPLICATION_H */