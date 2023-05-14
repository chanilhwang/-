/******************************************************************************/
/*!
\file   WEApplication.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

This is the Application layer for created and controlling a window for the
WarpEngine

*/
/******************************************************************************/
#ifndef WE_APPLICATION_H
#define WE_APPLICATION_H

/*! Used to exclude rarely-used stuff from Windows */
#define WIN32_LEAN_AND_MEAN 
#include <windows.h> /*HINSTANCE*/

//Forward Declarations
struct WEGameData;

/*! A Struct used to initialize the system. Students should create one of these
and pass it to InitializeSystem*/
struct WEInitData
{
  HINSTANCE   instance;    /*!< The Instance from WinMain*/
  const char* title;       /*!< The title of the window in windowed mode*/
  WEGameData* pGData;      /*!< A pointer that contains the intial game values*/
  int         gameDataSize;/*!< The size of the WEGameData struct*/
  int         height;      /*!< The height of the client area of the screen*/
  int         width;       /*!< The width of the client area of the screen*/
  int         fps;         /*!<*The target frames per second for the game, usually 30 or 60*/
  bool        fullScreen;  /*!< If the game should begin in fullscreen or not*/
};

//! Functions to Control the Window
namespace WEApplication
{
/*Call These in Main*/

/*This must be called first, before the game is started*/
void Init(const WEInitData& initStruct);
/*Call this after you add your states to start the game*/
void Update(void);
/*Call this after Update is finished*/
void Shutdown(void);

/*Call these to control The application*/

/*Use this to change to fullscreen and back*/
void SetFullScreen(bool fullScreen);
/*Use this to show and hide the window*/
void SetShowWindow(bool show);
/*Use this to show and hide the default window cursor*/
void SetShowCursor(bool showCursor);
/*Use this to change the resolution of the game*/
void ChangeResolution(int width, int height);

/*Call These to get information about the application.*/

/*Returns the height of the window (client area)*/
int GetHeight(void);
/*Returns the width of the window (client area)*/
int GetWidth(void);

}//end namespace WEApplication




#endif