/*******************************************************************************
filename    Main.cpp
author      Hwang, Chan Il
DP email    c.hwang@digipen.edu
course      CS230
assignment  1
due date    04/06/2017

Brief Description:
Create a simple window in Window OS.

*******************************************************************************/

#include<windows.h>
#include<tchar.h>

//Forward Declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain function
int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
				  	 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	//Main window class name
	static TCHAR szWindowClass[] = _T("c.hwangAssignment");

	//application title
	static TCHAR szTitle[] = _T("Assignment");

	//Define a window class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//Register the window class	
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return 1;
	}

	//Create a window
	HWND hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	//Displaying the created window
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	//Masage Loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

//Window procedure function
LRESULT CALLBACK WndProc(
				 HWND   hwnd,
				 UINT   uMsg,
				 WPARAM wParam,
				 LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, David!");

	switch (uMsg)
	{
	case WM_CREATE:
		return 0;

	case WM_COMMAND:
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}