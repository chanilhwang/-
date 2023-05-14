/**
 * @file CS_StageManager.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Stage Manager
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_StageManager.hpp"
#include "CS_Stages.hpp"
#include "CS_StageTypes.hpp"
#include "CS_Application.hpp"
#include "CS_Graphics.hpp"
#include "CS_StageBuildFactory.hpp"
#include "CS_AddStages.hpp"
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Timer_Frame.hpp"
#include <chrono>
#include <thread>
#include "CS_Physics.hpp"
#include "CS_SoundManager.hpp"
#include <stack>
#include "CS_GUI.hpp"

namespace StageManagerInfo
{
    struct PauseInfo
    {
        PauseInfo(CS_Stage* p_stage, CS_StageTypes type) :
            pStage(p_stage), type(type) {}
        CS_Stage* pStage;
        CS_StageTypes type;
    };
    std::stack<PauseInfo>        s_pauseStack;
    static CS_StageBuildFactory s_stageFactory;
    static CS_Stage*              s_pStage;       /*Pointer to base class stage */
    static CS_GameData*           s_pGameData;    /*Pointer to user defined shared data from main*/
    static CS_StageTypes          s_currtentStage;/*Enum to know what stage to load from factory*/
    static CS_StageTypes          s_nextStage;    /*Enum to know what stage to load next*/
    static bool                  s_isChanging;   /*TRUE is we are changing states, false otherwise*/
    static bool                  s_isQuitting;   /*TRUE if we are quitting, FALSE otherwise*/
    static bool                  s_isRestarting; /*TRUE if we are restarting, FALSE otherwise*/
    static bool                  s_isPausing;
    static bool                  s_isResuming;
    static bool                  s_drawPaused;   /*< True if we are pausing and want to draw the paused items*/
}


bool CS_StageManager::ShouldQuit()
{
	return StageManagerInfo::s_isQuitting;
}

void CS_StageManager::Quit()
{
	StageManagerInfo::s_isQuitting = true;
}

void CS_StageManager::AddStage(CS_StageTypes stageType, CS_StageBuilder * stageBuilder)
{
	StageManagerInfo::s_stageFactory.AddBuilderIntoFactory(stageType, stageBuilder);
}

void CS_StageManager::SetStartStage(CS_StageTypes startStageType)
{
	StageManagerInfo::s_currtentStage = startStageType;
	StageManagerInfo::s_nextStage = startStageType;
}

void CS_StageManager::SetNextStage(CS_StageTypes nextStage)
{
    StageManagerInfo::s_isChanging = true;
    StageManagerInfo::s_nextStage = nextStage;
}

void CS_StageManager::PauseAndSetNextStage(CS_StageTypes nextStage, bool drawPaused)
{
    StageManagerInfo::s_isPausing = true;
    StageManagerInfo::s_isChanging = true;
    StageManagerInfo::s_nextStage = nextStage;
    StageManagerInfo::s_drawPaused = drawPaused;
}

void CS_StageManager::Resume(void)
{
    if(StageManagerInfo::s_pauseStack.empty())
        return;
    StageManagerInfo::s_isChanging = true;
    StageManagerInfo::s_isResuming = true;
}

void CS_StageManager::Restart(void)
{
    StageManagerInfo::s_isRestarting = true;
}

CS_Camera * CS_StageManager::GetCameraOfCurrentLevel()
{
	return &StageManagerInfo::s_pStage->camera;
}

CS_GameData & CS_StageManager::GetGameData()
{
    return *StageManagerInfo::s_pGameData;
}

void CS_StageManager::Init(const CS_GameData& game_data)
{
	StageManagerInfo::s_pStage = nullptr;
	StageManagerInfo::s_pGameData = new CS_GameData(game_data);
	StageManagerInfo::s_currtentStage = CS_StageTypes::INVALID_Stage;
	StageManagerInfo::s_nextStage = CS_StageTypes::INVALID_Stage;
	
	StageManagerInfo::s_isQuitting = false;
	StageManagerInfo::s_isRestarting = false;
	StageManagerInfo::s_isPausing = false;
	StageManagerInfo::s_isResuming = false;
	StageManagerInfo::s_isChanging = true;//for init first stage
	//regester stage
	CS_AddStages();
}


void CS_StageManager::InitStage(void)
{
    if (StageManagerInfo::s_isRestarting)
    {
    	StageManagerInfo::s_pStage->Init();/*Call the initialize function*/
    	StageManagerInfo::s_isRestarting = false;/*We need to reset our restart flag*/
    }
    else if (StageManagerInfo::s_isResuming)
    {
        CS_ObjectManager::Resume();
    	CS_Physics::Resume();
    	CS_Graphics::Resume();
        StageManagerInfo::PauseInfo pi = StageManagerInfo::s_pauseStack.top();
        StageManagerInfo::s_pauseStack.pop();
        StageManagerInfo::s_currtentStage = StageManagerInfo::s_nextStage = pi.type;
        StageManagerInfo::s_pStage = pi.pStage;
        StageManagerInfo::s_isResuming = false;
        StageManagerInfo::s_isChanging = false;
    }
    else if (StageManagerInfo::s_isChanging)
    {
        StageManagerInfo::s_pStage = StageManagerInfo::s_stageFactory.BuildStageFromFactory(StageManagerInfo::s_currtentStage);
        StageManagerInfo::s_pStage->Init();/*Call the initialize function*/
        StageManagerInfo::s_isChanging = false;
    }
}

void CS_StageManager::Update(void)
{
    InitStage();
    while (!StageManagerInfo::s_isChanging && !StageManagerInfo::s_isQuitting && !StageManagerInfo::s_isRestarting)
    {
        CS_Timer_Frame::Update();
        CS_Input::Rest();
        CS_Application::PeekMessages();
        CS_GUI::NewFrame();//reset gui
		StageManagerInfo::s_pStage->camera.Update(CS_Timer_Frame::GetDt());
        CS_ObjectManager::Update(CS_Timer_Frame::GetDt());
        CS_Physics::Update();
        StageManagerInfo::s_pStage->Update(CS_Timer_Frame::GetDt());
        CS_Graphics::SetCameraFromStage(StageManagerInfo::s_pStage->camera);
        CS_Graphics::Update();//draw
        CS_SoundManager::Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    /*Change Stage*/
    ChangeStage();
}

void CS_StageManager::ChangeStage(void)
{
    if (StageManagerInfo::s_isPausing)
    {
        CS_ObjectManager::Pause();
        CS_Physics::Pause();
        CS_Graphics::Pause(StageManagerInfo::s_drawPaused);
        StageManagerInfo::PauseInfo pi(StageManagerInfo::s_pStage, StageManagerInfo::s_currtentStage);
        StageManagerInfo::s_pauseStack.push(pi);
        StageManagerInfo::s_isPausing = false;
    }
    else if (StageManagerInfo::s_isResuming)
    {
    	/*Make sure to shutdown the stage*/
    	StageManagerInfo::s_pStage->Shutdown();
    	delete StageManagerInfo::s_pStage;
    	StageManagerInfo::s_pStage = nullptr;
    }
    else if (!StageManagerInfo::s_isRestarting) //Just changine the stage
    {
    	///*Make sure to shutdown the stage*/
    	StageManagerInfo::s_pStage->Shutdown();
    	delete StageManagerInfo::s_pStage;
    	StageManagerInfo::s_pStage = nullptr;
    	//
    	////If we are setting the next state, that means we are ignore all
    	////paused states, so lets clear the pause stack
    	while (!StageManagerInfo::s_pauseStack.empty())
    	{
    	    CS_Graphics::Resume();
    	    CS_Physics::Resume();
    	    CS_ObjectManager::Resume();
    	    StageManagerInfo::PauseInfo pi = StageManagerInfo::s_pauseStack.top();
    	    pi.pStage->Shutdown();
    	    delete pi.pStage;
    	    StageManagerInfo::s_pauseStack.pop();
    	}
    
    }
    else if (StageManagerInfo::s_isRestarting)
    {
    	/*Make sure to shutdown the stage*/
    	StageManagerInfo::s_pStage->Shutdown();
    }
	
    StageManagerInfo::s_currtentStage = StageManagerInfo::s_nextStage;
}

void CS_StageManager::ShutDown()
{
    delete StageManagerInfo::s_pGameData;
    StageManagerInfo::s_pGameData = nullptr;
    StageManagerInfo::s_isQuitting = true;
}