/**
 * @file CS_StageManager.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Stage Manager
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_GameData.hpp"
#include "CS_StageTypes.hpp"
#include "CS_StageBuilder.hpp"
#include "CS_Camera.hpp"
class CS_Application;
class CS_StageManager
{
public:
    friend class CS_Application;
	static bool ShouldQuit();
	static void Quit();
	static void AddStage(CS_StageTypes stageType, CS_StageBuilder* stageBuilder);
	static void SetStartStage(CS_StageTypes startStageType);
	static void SetNextStage(CS_StageTypes nextStage);
	// Pauses the current stage, so it can be resuemd but chages stages
	static void PauseAndSetNextStage(CS_StageTypes nextStage, bool drawPaused = false);
	// Resumes the previous stage
	static void Resume(void);
	static void Restart(void);
	static CS_Camera* GetCameraOfCurrentLevel();
	static CS_GameData& GetGameData();
private:
    static void Init(const CS_GameData& game_data);
    static void InitStage(void);
    static void Update(void);
    static void ChangeStage(void);
    static void ShutDown(void);
	
	
};

