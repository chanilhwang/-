/**
 * @file CS_Editor.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Overall audio management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <string>
#include "CS_SoundTypes.hpp"
#include <fmod.hpp>
enum CS_SoundMode
{
	Loop_Normal,
	Default
};
class CS_Application;
class CS_StageManager;
class CS_SoundManager
{
	friend CS_Application;
	friend CS_StageManager;
public:
        static void InitializeSound(CS_SoundTypes types);
	static void RegisterSound(CS_SoundTypes types, std::string filename);
	static void SetSoundMode(CS_SoundTypes types, CS_SoundMode Mode);
	static void PlaySound(CS_SoundTypes types, bool pause=false);
    static void StopSound(CS_SoundTypes types);
    static void Pause(bool should_pause, CS_SoundTypes types);
	static FMOD::Channel* GetChannel(CS_SoundTypes types);
	static void SetMasterVolume(float volume);
	static void SetVolume(CS_SoundTypes types,float volume);
private:
    static void Init();
    static void Update();
};
