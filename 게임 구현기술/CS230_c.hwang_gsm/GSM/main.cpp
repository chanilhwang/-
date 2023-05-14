/******************************************************************************/
/*!
\file   main.cpp
\author David Ly
\par	dly
\par    email: dly\@digipen.edu
\par    CS230
\par    Game State Manager
\date   2017/05/17

Initial point for creating a window and intializing all related systems for the
game state manager assignment.
Template for CS230 GSM assignment

https://msdn.microsoft.com/en-us/library/bb384843.aspx
*/
/******************************************************************************/
#include "Application.h"
#include "System.h"
#include "GameStateManager.h"
#include "Input.h"

/******************************************************************************/
/*!
Main() used by the Windows API to run Win32 programs.

This is the replacement for main seen in traditional C/C++ programs.
The Windows API will always be looking for a WinMain function, so it
can be treated as such.  The function params are a bit different, so if
your program requires their usage, the information can be found on MSDN.

https://msdn.microsoft.com/library/windows/desktop/ms633559

\param HINSTANCE hInstance
A handle to the current instance of the application.

\param HINSTANCE hPrevInstance
A handle to the previous instance of the application. This parameter is always
NULL.

\param LPSTR     lpCmdLine
The command line for the application, excluding the program name.

\param int       nCmdShow
Controls how the window is to be shown.

\return
If the function succeeds, terminating when it receives a WM_QUIT message,
it should return the exit value contained in that message's wParam parameter.
If the function terminates before entering the message loop, it should return
zero.
*/
/******************************************************************************/
int CALLBACK WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
) {
	InitData initData;				/* My window init struct */
	initData.title = "CS230mA2";	/* Assignment title of the window */
	initData.hInstance = hInstance;	/* OS handle to the window */
	initData.fullscreen = false;	/* for the next assignment iteration */
	//initData.quit = false;			/* Quit application boolean */

	// Initialize function of the application window
	Application::InitWindowClass(initData);
	
	// -------------------------------------------------------------------------
	//Game State Manager Code Initialization
	// -------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	// Main game loop.  This handles the standard game loop behavior of
	//    initializing new game states, updating the current game states, then
	//    freeing the past game states as required.
	//
	// There is also a small section in the update loop where we do a call to
	//    the Windows API function PeekMessage.  This is a nonblocking function
	//    used to process any system messages that Windows is attempting to send
	//    to the process.
	//--------------------------------------------------------------------------
	// while (current game state is not QUIT)
	//{
		//----------------------------------------------------------------------
		// Initialization block.  This will load and intialize whatever the 
		//   current state is that is being loaded.  This uses the function
		//   pointers for GameStateLoad and GameStateInit, allowing for 
		//   different behavior for each state type.
		//----------------------------------------------------------------------
		//if (current game state does not equal RESTART)
		//{
			//Update game state
			//Load game state
		//}
		//else
			// Set the previous, current and next indicators to the same value

		//Initialize game state

		//-------------------------------------------------------------------------
		// Update block.  This will handle all behavior associated with updating
		//   the components of the game engine.  At this point, it includes the
		//   updates of the game state manager, as well as input, followed by
		//   the main call to draw all objects.  This uses the function pointers
		//   for GameStateUpdate and GameStateDraw, allowing for state-independent
		//   behavior.
		//
		// This section also contains a while loop for PeekMessage.  This function
		//   is a nonblocking function to process any messages in the queue.  This
		//   function will return a non-zero number if any messages remain,
		//   allowing us to only process messages when they exist.
		//-------------------------------------------------------------------------
		//while (current game state equals next game state
		//{
			// Windows API message loop, PeekMessage call

			//Input call
			//Update game state
			//Draw game state
		//}
		
		//-------------------------------------------------------------------------
		// Unload block.  This section handles all behavior associated with freeing
		//   objects in current states, as well as the unloading of the current
		//   state, before switching to the next state.
		//-------------------------------------------------------------------------
		//Free game state

		//if (next game state does not equal restart)
			//Unload game state

		//set previous game state to current game state
		//set current game state to next game state
	//} // end while (current game state is not QUIT)

	//---------------------------------------------------------------------------
	// System Exit.
	//---------------------------------------------------------------------------
	
	System_Initialize();
	GSM_Initialize(Level1);

	while (GetCurrentState() != Quit)
	{
		if (GetCurrentState() != Restart)
		{
			GSM_Update();
			GetLoad()();
		}
		else
		{
			SetPreviousState(GetCurrentState());
			SetNextState(GetCurrentState());
		}

		GetInitialize()();

		while (GetCurrentState() == GetNextState())
		{
			Application::Update();

			Input_Handle();
			GetUpdate()();
			GetDraw()();
		}

		GetFree()();

		if (GetNextState() != Restart)
			GetUnload()();

		SetPreviousState(GetCurrentState());
		SetCurrentState(GetNextState());
	}

	System_Exit();

	return 0;
}
