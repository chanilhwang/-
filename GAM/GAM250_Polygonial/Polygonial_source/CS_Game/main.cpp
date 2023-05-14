/**
 * @file main.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <Windows.h>
#include <CS_Application.hpp>
#include "CS_StageTypes.hpp"
#include "CS_StageManager.hpp"
#include "CS_JsonManager.hpp"
#include <iostream>

#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#endif
/**
 * @brief 
 * 
 * @param hInstance 
 * @param nCmdShow 
 * @return int wWinMain 
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
    ShowCursor(false);
    CS_JsonManager manager;
    auto res=manager.OpenFile("InitData.json");
    if(!res)
        std::cout << "fail to open initdata.json";
    CS_InitData init_data;
    CS_JsonObject obj;//get obj file
    manager.GetObj("CS_InitData", obj);
    obj.ReadData("Title", init_data.Title);
    obj.ReadData("StartStage", init_data.StartStage);
    obj.ReadData("Width", init_data.Width);
    obj.ReadData("Height", init_data.Height);
    CS_GameData game_data;

    init_data.GameData = game_data;
    CS_Application::Init(init_data);
    CS_StageManager::SetStartStage(CS_StageTypes::Splash_Stage);
    CS_Application::Update();
    CS_Application::EndAll();
    return 0;
}