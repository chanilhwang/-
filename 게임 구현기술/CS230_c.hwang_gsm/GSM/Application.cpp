/******************************************************************************/
/*!
\file   Application.cpp
\author David Ly
\par	dly
\par    email: dly\@digipen.edu
\par    CS230
\par    Game State Manager
\date   2017/05/17

This is the Application layer for creating and controlling a window for the
CS230, A globably accessable static class.

*/
/******************************************************************************/
#include "Application.h"
#include "GameStateManager.h"

namespace {
	/* The name of the Windows class. It is used to register and create the window */
	const char* CLASS_NAME = "CS230mA2";
	/* Window style of  windowed mode */
	const DWORD WINDOWED_STYLE = WS_POPUP | WS_CAPTION;	/* for next iteration */

	//"Private" class data
	const char* s_winTitle;		/* The title of the window */
	HINSTANCE   s_hInstance;	/* The instance from WinMain */
	WNDCLASSEX	s_windowClass;	/* The window class for creating the window */
	HWND        s_hwnd;			/* The window after creating it */
	DWORD       s_style;		/* The windows style */
	MSG			s_msg;
	bool		s_isFullScreen;	/* Fullscreen boolean */
}

/******************************************************************************/
/*!
Initializes the Application.  This MUST be called FIRST before doing
anything else.

\param [in] initData
A pointer to data struct about how to create the window.

\attention
You MUST call this FIRST before update or shutdown for your application.

\link
https://msdn.microsoft.com/library/windows/desktop/ms633559

*/
/******************************************************************************/
void Application::InitWindowClass(const InitData& initData) {
	
	/* Set up window data */
	s_winTitle = initData.title;
	s_hInstance = initData.hInstance;
	s_style = WINDOWED_STYLE;
	s_isFullScreen = initData.fullscreen;	//This is for the next iteration

	s_windowClass.cbSize = sizeof(WNDCLASSEX);
	s_windowClass.style = CS_HREDRAW | CS_VREDRAW;	/*Class style*/
													// CS_HREDRAW Redraws the entire window if a movement or size adjustment changes the width of the client area.
													// CS_VREDRAW Redraws the entire window if a movement or size adjustment changes the height of the client area.

	s_windowClass.lpfnWndProc = WndProc;	/*A function pointer which should point to the procedure function. Remember that the procedure function handles the window messages*/
	s_windowClass.cbClsExtra = 0;			/*The number of extra bytes you want to allocate for this window class structure. The default value is 0*/
	s_windowClass.cbWndExtra = 0;			/*The number of extra bytes you want to allocate for the window instance*/
	s_windowClass.hInstance = s_hInstance;	/* The unique handle to the application window by the OS. This is the 1st paramter passed to the WinMain function*/

	s_windowClass.hIcon = LoadIcon(s_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));	/*Handle to the icon class which will be displayed on the top left part of the window*/
	s_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);							/*Handle to the cursor class which will be used in this window class*/

	s_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	/*Handle to the class background brush. It can be a color value*/
	s_windowClass.lpszMenuName = NULL;							/*Pointer to a null terminated string which specifies the name of the menu resources used by this window class (if any)*/
	s_windowClass.lpszClassName = CLASS_NAME;					/*String that specifies thw window class name. This is needed to create any window based on this window class*/

	// Register the WNDCLASSEX with Windows OS so it knows about your window and how to send messages to it.
	RegisterClassEx(&s_windowClass);

	s_hwnd = CreateWindow(CLASS_NAME,	/*The class name we chose for the window class*/
		s_winTitle,						/*The window caption*/
		WS_OVERLAPPEDWINDOW,			/*The style of the window, which determines if the window will have a minimize/maximize buttons, if its frame is thick or not.. */
		CW_USEDEFAULT,					/*The X position of the top left corner of the window. Remember that (0,0) is the top left of the monitor*/
		CW_USEDEFAULT,					/*The Y position of the top left corner of the window. Remember that (0,0) is the top left of the monitor*/
		CW_USEDEFAULT,					/*The width of the window*/
		CW_USEDEFAULT,					/*The heiht of the window*/
		NULL,							/*Handle to the parent window (in case this was a child window)*/
		NULL,							/*Handle to a menu (In case there is a menu for this window)*/
		s_hInstance,						/*Instance of the module associated with the window. This is the 1st paramter passed to the WinMain function*/
		NULL);							/*Pointer to a value sent to the window in the WM_CREATE message*/

	ShowWindow(s_hwnd, SW_SHOWNORMAL);
	UpdateWindow(s_hwnd);
}

/******************************************************************************/
/*!
Gets Messages from windows and passes them to the WinProc. This must be called
once per frame.

\param [in] msg
A pointer to OS message to process.
*/
/******************************************************************************/
void Application::ProcessMessages(void)
{
	while (PeekMessage(&s_msg, s_hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&s_msg);
		DispatchMessage(&s_msg);
	}
}

/******************************************************************************/
/*!
Updates the window. This should be called once after
Application::InitWindowClass.

*/
/******************************************************************************/
void Application::Update(void) {
	ProcessMessages();
}

/******************************************************************************/
/*!
The function to handle all of the windows messages.

\param [in] hwnd
The window that received the message.

\param [in] msg
The message that was passed.

\param [in] wParam
The WPARAM this is filled with different data for each message.

\param [in] lParam
The LPARAM This is filled with different data for each message.

\return
The result of the default windows procedure or 0.
*/
/******************************************************************************/
LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;		/* device context */
	HDC hdc;			/* the paint struct */

	switch (msg) {

		/* when the window is created */
	case WM_CREATE:
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 100, 100, 200, 200);
		EndPaint(hwnd, &ps);
		break;

	/* When it's time for the window to be closed and removed */
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			SetNextState(Quit); // Quit the application by setting the next game state indicator to the QUIT "state"
		break;

		/* called any time the window is moved */
	case WM_MOVE:
		/* Invalidate the rect to force a redraw */
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	} // end switch (msg) {

	return 0;
}
