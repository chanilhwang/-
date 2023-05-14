/**
 * @file Graphics_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Graphics Component
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_Camera.hpp"
class CS_StageManager;
class CS_Stage
{
public:
	friend CS_StageManager;
    //!Used to Load big resource such as textures in an CS_Stage.  This is only called once.
    CS_Stage(void) {};
    //!Used to Unload textures or Save data to file.  This is only called when we are not restarting.
    virtual ~CS_Stage(void) {}//emtpy virtual destructor
    //!Used to Set/Reset data when the CS_Stage starts or restarts
    virtual void Init(void) = 0;
    //!Called once per frame to update CS_Stage Logic
    virtual void Update(float dt) = 0;
    //!Called after a Stage Change or request to restart
    virtual void Shutdown(void) = 0;
protected:
	CS_Camera camera;
};
