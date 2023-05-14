/**
 * @file CS_GUI.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief IMGUI usage.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_StageManager.hpp"

class CS_Object;
class CS_Application;
class CS_Graphics;
class CS_GUI
{
    friend CS_StageManager;
    friend CS_Application;
    friend CS_Graphics;
private:
    static void Init(void* MainWindow);
    static void NewFrame();
    
    static void Draw();
    static void ShutDown();
};