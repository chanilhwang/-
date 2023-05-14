/******************************************************************************/
/*!
\file   Application.cpp
\author Your name
\par    email: Your email (example: dly\@digipen.edu)
\par    CS230
\par    Simple Windows Creation mA2
\date   Today's date

This is the Application layer for creating and controlling a window for the
CS230, A globably accessable static class.

*/
/******************************************************************************/
#include "Application.h"

/* The namespace will help protect the data from being used outside of this file */
namespace {
	/* The name of the Windows class. It is used to register and create the window */
	const char* CLASS_NAME = "CS230mA2";

	//"Private" class data
	const char* s_winTitle;		/* The title of the window */
	HINSTANCE   s_hinstance;	/* The instance from WinMain */
	WNDCLASSEX	s_wndclassex;	/* The window class for creating the window */
	HWND        s_hWnd;			/* The window after creating it */	
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
	
	/* Define and set up the window data */
	s_winTitle = initData.title;
	s_hinstance = initData.hInstance;

	// Register the WNDCLASSEX with Windows OS so it knows about your window and how to send messages to it.
	s_wndclassex.cbSize = sizeof(WNDCLASSEX);
	s_wndclassex.style = CS_HREDRAW | CS_VREDRAW;
	s_wndclassex.lpfnWndProc = WndProc;
	s_wndclassex.cbClsExtra = 0;
	s_wndclassex.cbWndExtra = 0;
	s_wndclassex.hInstance = s_hinstance;
	s_wndclassex.hIcon = LoadIcon(s_hinstance, MAKEINTRESOURCE(IDI_APPLICATION));
	s_wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	s_wndclassex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	s_wndclassex.lpszMenuName = NULL;
	s_wndclassex.lpszClassName = CLASS_NAME;
	s_wndclassex.hIconSm = LoadIcon(s_wndclassex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&s_wndclassex);


	// Create a window
	s_hWnd = CreateWindow(
		CLASS_NAME,
		s_winTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		s_hinstance,
		NULL
	);

	ShowWindow(s_hWnd,
		SW_SHOW);
	UpdateWindow(s_hWnd);

	// Display the created window (Show and Update Window)
}

/******************************************************************************/
/*!
Gets Messages from windows and passes them to the WinProc. This must be called
once per frame.

\param [in] msg
A pointer to OS message to process.
*/
/******************************************************************************/
void Application::ProcessMessages(MSG& msg)
{
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.wParam == WM_QUIT)
			break;
	}
}

/******************************************************************************/
/*!
The function to process all of the windows messages.

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
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc,
			5, 5,
			"Hello, David!", 14);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}

	return 0;
}