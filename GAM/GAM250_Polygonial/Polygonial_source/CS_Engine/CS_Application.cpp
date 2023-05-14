#include "CS_Application.hpp"
#include "CS_Graphics.hpp"
#include "GL/glew.h"
#include "GL/wglew.h"
#include "CS_StageManager.hpp"
#include <iostream>
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "CS_GUI.hpp"
#include "CS_SoundManager.hpp"
#include "resource.h"
#include "CS_Option.hpp"

const wchar_t* const OpenglClassName = L"OpenglClass";
HINSTANCE IntsanceHandle = nullptr;
int width, height;
HWND MainWindow = nullptr;
HDC DeviceContext = nullptr;
HGLRC OpenglContext = nullptr;
bool Quit = false;
//for test


/**
 * @brief Initialize application
 * 
 * @param initData Initialize data.
 * @return true If initialization was successful.
 * @return false If failed to create window.
 */
bool CS_Application::Init(CS_InitData& initData)
{
    if (IntsanceHandle == nullptr)
        IntsanceHandle = GetModuleHandle(nullptr);
    MakeOldOpenglWindow();
    RegisterOpenglClass();
    std::wstring wintitle;
    wintitle.assign(initData.Title.begin(), initData.Title.end());
	if(!initData.IsFullScreen)
		MainWindow = CreateWindowEx(0L, OpenglClassName, wintitle.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, initData.Width, initData.Height, nullptr, nullptr, IntsanceHandle, nullptr);
	else
		MainWindow = CreateWindowEx(0L, OpenglClassName, wintitle.c_str(), WS_POPUP, 0, 0, initData.Width, initData.Height, nullptr, nullptr, IntsanceHandle, nullptr);
	width = initData.Width;
    height = initData.Height;
    if (MainWindow == nullptr)
        return false;
    //get dc for opengl
    DeviceContext = GetDC(MainWindow);
    //make openglcontext
    MakeOpenGLContext();
    //make it current
    wglMakeCurrent(DeviceContext, OpenglContext);
    ShowWindow(MainWindow, SW_MAXIMIZE);
    UpdateWindow(MainWindow);
    wglSwapIntervalEXT(1);//v sync on

    CS_Graphics::init();
    CS_StageManager::Init(initData.GameData);
    CS_ObjectManager::Init();
    CS_Input::Init();
    CS_GUI::Init(MainWindow);
    //test
    CS_SoundManager::Init();
	CS_SoundManager::SetMasterVolume(initData.volume);
	CS_Options::Init(glm::vec2(width, height),
		initData.IsFullScreen,
		initData.volume);
    return true;
}
/**
 * @brief Update stage manager and check if it should quit.
 * 
 */
void CS_Application::Update()
{
    while (!Quit)
    {

        CS_StageManager::Update();
        if (CS_StageManager::ShouldQuit())
            ShutDown();
    }
}
/**
 * @brief Shut down the application.
 * 
 */
void CS_Application::ShutDown()
{
    PostMessage(MainWindow, WM_CLOSE, 0, 0);
    Quit = true;
}
/**
 * @brief Finish all currently working programs.
 * 
 */
void CS_Application::EndAll()
{
    CS_GUI::ShutDown();
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(OpenglContext);
    ReleaseDC(MainWindow, DeviceContext);
    CS_StageManager::ShutDown();
}
/**
 * @brief 
 * 
 * @return PIXELFORMATDESCRIPTOR 
 */
PIXELFORMATDESCRIPTOR CS_Application::MakePixelDescription()
{
    PIXELFORMATDESCRIPTOR pfd;
    //for safe
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;
    return pfd;
}
/**
 * @brief Make an old OpenGL window.
 * 
 * @return true If making window was successful. 
 * @return false If failed to create window.
 */
bool CS_Application::MakeOldOpenglWindow()
{
    //window class 
    const wchar_t* const OldClassName = L"OldOpenglClass";
    WNDCLASSEX OldClass{};
    OldClass.cbSize = sizeof(WNDCLASSEX);
    OldClass.style = CS_OWNDC;
    OldClass.lpfnWndProc = &DefWindowProc;
    OldClass.cbClsExtra = 0;
    OldClass.cbWndExtra = 0;
    OldClass.hInstance = IntsanceHandle;
    OldClass.hIcon = nullptr;
    OldClass.hCursor = nullptr;
    OldClass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    OldClass.lpszMenuName = nullptr;
    OldClass.lpszClassName = OldClassName;
    OldClass.hIconSm = nullptr;
    //register class 
    RegisterClassEx(&OldClass);
    //make window 
    HWND OldGLWindow = CreateWindowEx(0L, OldClassName, L"OldOpenglWindow", WS_TILEDWINDOW, 0, 0, 32, 32, nullptr, nullptr, IntsanceHandle, nullptr);
    //Get DC for GL
    HDC OldDeviceContext = GetDC(OldGLWindow);
    //Make PixelDescription
    PIXELFORMATDESCRIPTOR PixelDescription = MakePixelDescription();
    //it will be used for set pixelformat 
    auto IPixelFormat = ChoosePixelFormat(OldDeviceContext, &PixelDescription);
    //set pixel format
    SetPixelFormat(OldDeviceContext, IPixelFormat, &PixelDescription);
    //make old opengl context
    HGLRC OldOpenglContext = wglCreateContext(OldDeviceContext);
    //make it current 
    wglMakeCurrent(OldDeviceContext, OldOpenglContext);
    //now i can init glew
    glewInit();
    //destory old things 
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(OldOpenglContext);
    ReleaseDC(OldGLWindow, OldDeviceContext);
    DestroyWindow(OldGLWindow);
    UnregisterClass(OldClassName, IntsanceHandle);

    return true;
}
/**
 * @brief Register OpenGL class and initialize.
 * 
 */
void CS_Application::RegisterOpenglClass()
{
    WNDCLASSEX winclass{};
    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.style = CS_OWNDC;
    winclass.lpfnWndProc = WinProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = IntsanceHandle;
    // default icon
    winclass.hIcon = LoadIcon(IntsanceHandle, MAKEINTRESOURCE(IDI_ICON1));
    // default cursor
    winclass.hCursor = nullptr;
    // default background color
    winclass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    // no menus
    winclass.lpszMenuName = nullptr;
    winclass.lpszClassName = OpenglClassName;
    // set the icon
    winclass.hIconSm = LoadIcon(IntsanceHandle, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClassEx(&winclass);
}
/**
 * @brief Making OpenGL context.
 * 
 * @return true If successful.
 * @return false If failed to create context.
 */
bool CS_Application::MakeOpenGLContext()
{
    const int new_pixel_format_attributes[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0 // End of attributes list
    };

    constexpr unsigned number_of_matching_formats_we_want_to_examine = 1;
    int pixel_formats[number_of_matching_formats_we_want_to_examine] = { 0 };
    unsigned number_of_matching_formats_found;

    wglChoosePixelFormatARB(DeviceContext, new_pixel_format_attributes, nullptr,
        number_of_matching_formats_we_want_to_examine, &pixel_formats[0],
        &number_of_matching_formats_found);

    PIXELFORMATDESCRIPTOR oldPixelFormat = MakePixelDescription();

    auto format_is_set = SetPixelFormat(DeviceContext, pixel_formats[0], &oldPixelFormat);

    if (format_is_set == FALSE)
        return false;

    int context_attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        3,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        3,
        WGL_CONTEXT_FLAGS_ARB,
        WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // we want to use version 3.3 and later functions
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, // we don't want nor need COMPATIBILITY with older versions of OpenGL
        0 // End of attributes list
    };

    OpenglContext = wglCreateContextAttribsARB(DeviceContext, nullptr, context_attributes);

    return OpenglContext != nullptr;
}

/**
 * @brief Set a virtual key to a corresponding keyboard button.
 * 
 * @param key int of the virtual key.
 * @return InputButtons The corresponding keyboard button.
 */
InputButtons VirtualKeyToKeyboardButton(int key)
{
	switch (key)
	{
	case VK_ESCAPE:
		return InputButtons::Escape;
	case VK_SPACE:
		return InputButtons::Space;
	case VK_LEFT:
		return InputButtons::Left;
	case VK_UP:
		return InputButtons::Up;
	case VK_RIGHT:
		return InputButtons::Right;
	case VK_DOWN:
		return InputButtons::Down;
	case '0':
		return InputButtons::_0;
	case '1':
		return InputButtons::_1;
	case '2':
		return InputButtons::_2;
	case '3':
		return InputButtons::_3;
	case '4':
		return InputButtons::_4;
	case '5':
		return InputButtons::_5;
	case '6':
		return InputButtons::_6;
	case '7':
		return InputButtons::_7;
	case '8':
		return InputButtons::_8;
	case '9':
		return InputButtons::_9;
	case 'A':
		return InputButtons::A;
	case 'B':
		return InputButtons::B;
	case 'C':
		return InputButtons::C;
	case 'D':
		return InputButtons::D;
	case 'E':
		return InputButtons::E;
	case 'F':
		return InputButtons::F;
	case 'G':
		return InputButtons::G;
	case 'H':
		return InputButtons::H;
	case 'I':
		return InputButtons::I;
	case 'J':
		return InputButtons::J;
	case 'K':
		return InputButtons::K;
	case 'L':
		return InputButtons::L;
	case 'M':
		return InputButtons::M;
	case 'N':
		return InputButtons::N;
	case 'O':
		return InputButtons::O;
	case 'P':
		return InputButtons::P;
	case 'Q':
		return InputButtons::Q;
	case 'R':
		return InputButtons::R;
	case 'S':
		return InputButtons::S;
	case 'T':
		return InputButtons::T;
	case 'U':
		return InputButtons::U;
	case 'V':
		return InputButtons::V;
	case 'W':
		return InputButtons::W;
	case 'X':
		return InputButtons::X;
	case 'Y':
		return InputButtons::Y;
	case 'Z':
		return InputButtons::Z;
	case VK_NUMPAD0:
		return InputButtons::NumPad_0;
	case VK_NUMPAD1:
		return InputButtons::NumPad_1;
	case VK_NUMPAD2:
		return InputButtons::NumPad_2;
	case VK_NUMPAD3:
		return InputButtons::NumPad_3;
	case VK_NUMPAD4:
		return InputButtons::NumPad_4;
	case VK_NUMPAD5:
		return InputButtons::NumPad_5;
	case VK_NUMPAD6:
		return InputButtons::NumPad_6;
	case VK_NUMPAD7:
		return InputButtons::NumPad_7;
	case VK_NUMPAD8:
		return InputButtons::NumPad_8;
	case VK_NUMPAD9:
		return InputButtons::NumPad_9;
	case VK_F5:
		return InputButtons::F_5;
	case VK_CONTROL:
		return InputButtons::Ctrl;
	default:
		return InputButtons::None;
	}
}
/**
 * @brief For imgui initialization.
 * 
 * @param hWnd 
 * @param msg 
 * @param wParam 
 * @param lParam 
 * @return LRESULT 
 */
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
/**
 * @brief 
 * 
 * @param hWnd 
 * @param message 
 * @param wParam 
 * @param lParam 
 * @return LRESULT 
 */
LRESULT CS_Application::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_SIZE:
    {
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        CS_Graphics::SetDisplayAreaSize(width, height);
    }
    break;
    case WM_CLOSE:
    {
        /*if (event_handler != nullptr)
            event_handler->HandleWindowClose();*/
	CS_StageManager::ShutDown();
        DestroyWindow(hWnd);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
    case WM_PAINT:
    {
        BeginPaint(hWnd, nullptr);
        EndPaint(hWnd, nullptr);
    }
    break;
    case WM_KEYDOWN:
    {
	CS_Input::SetPressed(VirtualKeyToKeyboardButton((int)wParam), true);
    }
    break;
    case WM_KEYUP:
    {
	CS_Input::SetPressed(VirtualKeyToKeyboardButton((int)wParam), false);
    }
    break; 
    case WM_MOUSEMOVE:
		CS_Input::SetMouse(int(LOWORD(lParam)), int(HIWORD(lParam)));
        break;
    case WM_MOUSEWHEEL:
	{
		short direction = GET_WHEEL_DELTA_WPARAM(wParam);

		if (direction > 0)
		{
			CS_Input::SetPressed(InputButtons::Mouse_MiddleUp, true);
			/*Set it to off so it is only active this frame*/
			CS_Input::SetPressed(InputButtons::Mouse_MiddleUp, false);
		}
		else if (direction < 0)
		{
			CS_Input::SetPressed(InputButtons::Mouse_MiddleDown, true);
			/*Set it to off so it is only active this frame*/
			CS_Input::SetPressed(InputButtons::Mouse_MiddleDown, false);
		}
		CS_Input::SetMouseWheelScrollAmount(direction);
	}
        break;
    case WM_LBUTTONDOWN:
        CS_Input::SetPressed(InputButtons::Mouse_Left, true);
        break;
    case WM_MBUTTONDOWN:
        CS_Input::SetPressed(InputButtons::Mouse_Middle, true);
        break;
    case WM_RBUTTONDOWN:
        CS_Input::SetPressed(InputButtons::Mouse_Right, true);
        break;
    case WM_LBUTTONUP:
        CS_Input::SetPressed(InputButtons::Mouse_Left, false);
        break;
    case WM_MBUTTONUP:
        CS_Input::SetPressed(InputButtons::Mouse_Middle, false);
        break;
    case WM_RBUTTONUP:
        CS_Input::SetPressed(InputButtons::Mouse_Right, false);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
/**
 * @brief 
 * 
 */
void CS_Application::PeekMessages()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != FALSE)
    {
        if (msg.message == WM_QUIT) // note WM_QUIT doesn't get sent to the WnfProc, which is why we check it here
            Quit = true;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
/**
 * @brief 
 * 
 */
void CS_Application::SwapBuffer()
{
    SwapBuffers(DeviceContext);
}
/**
 * @brief Get the current window size.
 * 
 * @return glm::vec2 Current window size.
 */
glm::vec2 CS_Application::GetWindowSize()
{
    return glm::vec2(width,height);
}

void CS_Application::Resize(int new_width, int new_height)
{
	width = new_width;
	height = new_height;
	CS_Graphics::SetDisplayAreaSize(width, height);
}

void CS_Application::ChangeToFullScreen(bool IsFullScreen)
{
	if (IsFullScreen) {
		SetWindowLong(MainWindow, GWL_STYLE, WS_POPUP);
		ShowWindow(MainWindow, SW_SHOW);
	}
	else {
		SetWindowLong(MainWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		ShowWindow(MainWindow, SW_SHOW);
	}
}