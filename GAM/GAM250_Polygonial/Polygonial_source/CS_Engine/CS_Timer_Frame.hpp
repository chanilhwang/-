/**
 * @file CS_Timer_Frame.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Timer Frame
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once

class CS_Timer_Frame
{
public:
	static void Reset();
	static double GetElapsedSeconds();
	static void Update();
	static float GetDt();
	static int GetFPS();
};

