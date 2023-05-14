/******************************************************************************/
/*!
\file   WEDebugTools.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26


This file contains the prototyes for the Debug functions and Macros.

*/
/******************************************************************************/
#ifndef WE_DEBUGTOOLS_H
#define WE_DEBUGTOOLS_H

/*Debug helper functions*/
/*This is the WriteText function prototyped from Graphics.*/
namespace WEGraphics
{
  void WriteText(const char* text, float x, float y);
}

//! Tools used to help debug the game
namespace WEDebug
{
/*These functions should NOT be called!!!! USE THE MACROS!!!! */
bool Assert(bool expression, const char* outputMessage,
  const char* fucntionName, const char* fileName,
  unsigned lineNumber);
void CreateConsole(void);
void DestroyConsole(void);
void ClearScreen(void);
void TestResult(bool expression, const char* testName);

/*This should only be called for IMPORTANT MESSAGES to the user
Really STUDENTS probably don't need it!!! */
int MessagePopup(const char* outputMessage);
}




/*Macros for debug only!!!*/
#if defined(DEBUG) | defined(_DEBUG)
/*! Used to exclude rarely-used stuff from Windows */
#define WIN32_LEAN_AND_MEAN 
#include <windows.h> /*DebugBreak*/
#include <cstdio>
#include <cstdlib>
/*! Use this macro instead of the function to print to the console in debug
only*/
#define WEDEBUG_PRINT(...) printf(__VA_ARGS__)
/*! Use this macro instead of the function to clear the console in debug only*/
#define WEDEBUG_CLEAR() WEDebug::ClearScreen()
/*! Use this macro instead of the function to ASSERT in debug only*/
#define WEDEBUG_ASSERT(exp, str) if(WEDebug::Assert((exp),(str),      \
  __FUNCTION__,__FILE__, __LINE__)) {DebugBreak();}         
/*!Use this macro instead of the function to create a console in debug only*/
#define WEDEBUG_CREATE_CONSOLE() WEDebug::CreateConsole();
/*!Use this macro instead of the function to destroy a console in debug only*/
#define WEDEBUG_DESTROY_CONSOLE() WEDebug::DestroyConsole();
/*! Use this macro to draw debug text on the screen*/
#define WEDEBUG_WRITETEXT(text, x, y) WEGraphics::WriteText((text),(x), (y))
/*!Use this to test your code in debug only*/
#define WEDEBUG_TEST(exp, str) WEDebug::TestResult((exp), (str))
#else
/*! Use this macro instead of the function to print to the console in debug
only*/
#define WEDEBUG_PRINT(...)
/*! Use this macro instead of the function to clear the console in debug only*/
#define WEDEBUG_CLEAR()
/*! Use this macro instead of the function to ASSERT in debug only*/
#define WEDEBUG_ASSERT(expression, outputMessage)
/*!Use this macro instead of the function to create a console in debug only*/
#define WEDEBUG_CREATE_CONSOLE()
/*!Use this macro instead of the function to destroy a console in debug only*/
#define WEDEBUG_DESTROY_CONSOLE()
/*! Use this macro to draw debug text on the screen*/
#define WEDEBUG_WRITETEXT(text, x, y)
/*!Use this to test your code in debug only*/
#define WEDEBUG_TEST(exp, str) 
#endif


#endif