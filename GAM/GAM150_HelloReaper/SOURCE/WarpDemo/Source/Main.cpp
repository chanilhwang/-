/******************************************************************************
FileName: Main.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/ 
#include "WEMemory.h"

#include <windows.h> /*WinMain*/ 

/*Include the engine functions*/
#include "WEApplication.h"
#include "WEStateManager.h"
#include "WEState.h"
#include "WEGameData.h"

/*My GameStates*/
#include "WEDebugTools.h"

//PLEASE MIND THE GAME FLOW!
#include "splash_screen.h"
#include "main_menu.h"
#include "game_play_level.h"
#include "upgrade_level.h"
#include "ending.h"
#include "tutorial.h"

void AddStates(void)
{
    WEState state;
    int stateID;

    //Add splash state
    state.Load = SplashLoad;
    state.Init = SplashInit;
    state.Update = SplashUpdate;
    state.Shutdown = SplashShutdown;
    state.Unload = SplashUnload;
    stateID = WEStateManager::AddState(state);

	//Set as a starting state  
	WEStateManager::SetStartState(stateID);

    //Add mainmenu state
    state.Load = MainMenuLoad;
    state.Init = MainMenuInit;
    state.Update = MainMenuUpdate;
    state.Shutdown = MainMenuShutdown;
    state.Unload = MainMenuUnload;
	stateID = WEStateManager::AddState(state);

	//Add tutorial state
	state.Load = tutorialLoad;
	state.Init = tutorialInit;
	state.Update = tutorialUpdate;
	state.Shutdown = tutorialShutdown;
	state.Unload = tutorialUnload;
	stateID = WEStateManager::AddState(state);

    //Add gameplay state
    state.Load = GamePlayingLoad;
    state.Init = GamePlayingInit;
    state.Update = GamePlayingUpdate;
    state.Shutdown = GamePlayingShutdown;
    state.Unload = GamePlayingUnload;
    stateID = WEStateManager::AddState(state);

    state.Load = UpgradeLevelLoad;
    state.Init = UpgradeLevelInit;
    state.Update = UpgradeLevelUpdate;
    state.Shutdown = UpgradeLevelShutdown;
    state.Unload = UpgradeLevelUnload;
    stateID = WEStateManager::AddState(state);

    state.Load = EndingLoad;
	state.Init = EndingInit;
	state.Update = EndingUpdate;
	state.Shutdown = EndingShutdown;
	state.Unload = EndingUnload;
	stateID = WEStateManager::AddState(state);
}

/******************************************************************************/
/*!

\brief
The main function for a windowed program.

\param instance 
An identifier for the process.  This is used for various windows things.

\param prev
This is not used anymore, we can ignore it.

\param comamndLine
A string that is comes from the typed command line.  In games we usually don't
care.

\param show 
A variable stating if the window is visible, we always want it visible.

\return
An Error code.  Just return 0;
*/
/******************************************************************************/
#pragma warning(suppress: 28251)
int WINAPI WinMain(HINSTANCE instance,
                   HINSTANCE /*prev*/, 
                   LPSTR /*commandLine*/, 
                   int /*show*/)
{
    /*Declare my InitStruct*/
    WEInitData initStruct;

    /*Create my game data initial values*/
    WEGameData gameData = { 0 };

    gameData.PlayerShield = 0;
    gameData.PlayerBullet = 0;
    gameData.PlayerEnergy = 5;
    gameData.PlayerUpgrades[6] = {0};
    gameData.ScreenNumber = 2;
    gameData.FullScreen = false;
    gameData.CurrentLevelID = -1;

    gameData.BGMVolume = 50.0f;
    gameData.SFXVolume = 50.0f;
    gameData.PlayTime = 0.0f;

    /*Set up my InitStruct*/
    initStruct.instance   = instance;
    initStruct.width      = 1280;
    initStruct.height     = 720;
    initStruct.title      = "Hello Reaper";
    initStruct.fps        = 60;
    initStruct.fullScreen = gameData.FullScreen;

    /*Information about your specific gamedata */
    initStruct.pGData       = &gameData;
    initStruct.gameDataSize = sizeof(WEGameData);
    
    /*Pass InitStruct to Function.  This function must be called first!!!*/
    WEApplication::Init(initStruct);
    /*Function to add all of my game states*/
    AddStates();
    /*Start running the game*/
    WEApplication::Update();
    /*This function must be called after the window has closed!!!*/

    WEApplication::Shutdown();

    return 0;
}