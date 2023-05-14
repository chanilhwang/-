/**
 * @file CS_ObjectManager.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Edit objects using component and archetypes.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_ComponentTypes.hpp"
#include "CS_ArcheTypes.hpp"
#include "CS_ComponentBuilder.hpp"
#include "CS_Particle.hpp"
class CS_Object;
class CS_Application;
class CS_StageManager;
class CS_JsonManager;
class CS_ObjectManager
{
	friend CS_Application;
	friend CS_StageManager;
public:
    static void LoadObjectFromFile(const char* jsonPath);
	static void WriteObejctToFile(CS_JsonManager& json_manager);
    static void AddArcheType(CS_ArcheTypes type, const char* jsonPath);
    static void AddObject(CS_Object* objToAdd);
    static CS_Object* CreateObjectWithArcheType(CS_ArcheTypes type);
	static std::vector<CS_Object*> CreateParticles(int amount, CS_ArcheTypes type);
    static void DestoryObject(CS_Object *objToDel);
    static void DestroyAllObjects(bool destroyPaused);
	static CS_Component* CreateComponentWithComponentType(CS_ComponentTypes type);
    static void AddComponent(CS_ComponentTypes type, CS_ComponentBuilder* pBuilder);
    static CS_Object* GetMouseInObj();
	static CS_Object* GetFirstObjectByType(CS_ArcheTypes type);
	static CS_Object* GetFirstObjectByName(const std::string& name);
private:
	static void Init();
	static void Update(float dt);
    static void Pause();
    static void Resume();
};