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
#include "CS_SoundManager.hpp"
#include <iostream>
#include <fmod.hpp>
#include <vector>
#include <array>
#include <unordered_map>
#include "CS_AddSounds.hpp"
#include <glm/glm.hpp>

namespace SoundManagerInfo
{
    FMOD::System *system = nullptr;
	std::unordered_map<CS_SoundTypes, FMOD::Sound*> sounds;
	std::array<FMOD::Channel*, 512> channels{};
	//std::vector<FMOD::Sound*> sounds;
    //FMOD::Sound * sound = nullptr;
}


void CS_SoundManager::RegisterSound(CS_SoundTypes types, std::string filename)
{
	FMOD::Sound *temp;
	auto res = SoundManagerInfo::system->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &temp);
	if (res != FMOD_OK)
	{
		std::cout << "create sound fail";
		return;
	}
	SoundManagerInfo::sounds.insert(std::make_pair(types, temp));

}

void CS_SoundManager::SetSoundMode(CS_SoundTypes types, CS_SoundMode Mode)
{
	if (Mode==Loop_Normal)
		SoundManagerInfo::sounds[types]->setMode(FMOD_LOOP_NORMAL);
	else if(Mode==Default)
		SoundManagerInfo::sounds[types]->setMode(FMOD_DEFAULT);
}

void CS_SoundManager::PlaySound(CS_SoundTypes types, bool pause)
{
	SoundManagerInfo::system->playSound(SoundManagerInfo::sounds[types], nullptr, pause, &SoundManagerInfo::channels[static_cast<int>(types)]);
}


void CS_SoundManager::InitializeSound(CS_SoundTypes types)
{
}

FMOD::Channel * CS_SoundManager::GetChannel(CS_SoundTypes types)
{
	return SoundManagerInfo::channels[static_cast<int>(types)];
}

void CS_SoundManager::SetMasterVolume(float volume)
{
	volume = glm::clamp(volume, 0.f, 1.f);
	for (int i=0; i< static_cast<int>(CS_SoundTypes::INVALID_Sound); ++i)
	{
		SoundManagerInfo::channels[i]->setVolume(volume);
	}
}

void CS_SoundManager::SetVolume(CS_SoundTypes types, float volume)
{
	volume = glm::clamp(volume, 0.f, 1.f);
	SoundManagerInfo::channels[static_cast<int>(types)]->setVolume(volume);
}


void CS_SoundManager::Pause(bool is_pause, CS_SoundTypes types)
{
    SoundManagerInfo::channels[static_cast<int>(types)]->setPaused(is_pause);
}

void CS_SoundManager::StopSound(CS_SoundTypes types)
{
    SoundManagerInfo::channels[static_cast<int>(types)]->stop();
}



void CS_SoundManager::Init()
{
    auto res = FMOD::System_Create(&SoundManagerInfo::system);
    if(res!=FMOD_OK)
       std::cout << "system create fail";
    res = SoundManagerInfo::system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (res != FMOD_OK)
        std::cout << "system init fail";
	CS_AddSounds();
}

void CS_SoundManager::Update()
{
    SoundManagerInfo::system->update();
}
