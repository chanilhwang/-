/**
 * @file CS_ObjectManager.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Edit objects using component and archetypes.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_ObjectManager.hpp"
#include "CS_JsonManager.hpp"
#include "CS_ComponentBuildFactory.hpp"
#include "CS_AddComponents.hpp"
#include "CS_Object.hpp"
#include <algorithm>
#include <vector>
#include <stack>
#include <iostream>
#include "CS_AddArcheTypes.hpp"
#include "CS_ComponentTypes.hpp"
#include <sstream>
#include "CS_ColliderComponent.hpp"
#include "CS_Physics.hpp"
#include "CS_Particle.hpp"
namespace ObjectManagerInfo
{
	typedef std::unordered_map<CS_ArcheTypes,CS_Object*>ArcheTypeMap; //typedef Container to hold prototypes objects
	ArcheTypeMap s_archetypes;
    CS_ComponentBuildFactory s_componentFactory;
    std::vector<CS_Object*> s_objects;
    int s_objectStart;//The value to start updating the object list from.
    std::stack<int> s_pauseStack;
	ParticleGenerator particleGen;
}

void CS_ObjectManager::LoadObjectFromFile(const char* jsonPath)
{
    CS_JsonManager json_manager;
    json_manager.OpenFile(jsonPath);
	std::vector<CS_JsonManager> json_objects;
	json_manager.ReadArray("objects", json_objects);
	int size = (int)json_objects.size();
	for (int i=0; i<size;++i)
	{
		CS_Object* pObj = new CS_Object(CS_ArcheTypes::NewObject);
		pObj->ReadFromFile(json_objects[i]);
		std::stringstream ss;
		json_objects[i].ReadData("ComponentsToBeAdded", ss);
		std::string Component;
		while (ss >> Component)
		{
			CS_Component*p_component =
				ObjectManagerInfo::s_componentFactory.BuildComponentFromFactory(StringToComponent(Component));
			
			CS_JsonObject File;
			json_objects[i].GetObj(Component.c_str(), File);
			p_component->ReadFromFile(File);
			pObj->AddComponent(p_component);
			CS_ColliderComponent* p_collider_component = dynamic_cast<CS_ColliderComponent*>(p_component);
			if(p_collider_component)
				CS_Physics::AddColliderComponent(p_collider_component);
		}
		ObjectManagerInfo::s_objects.push_back(pObj);
	}
}

void CS_ObjectManager::WriteObejctToFile(CS_JsonManager& json_manager)
{
	for (size_t i = ObjectManagerInfo::s_objectStart; i < ObjectManagerInfo::s_objects.size(); ++i)
	{
		CS_JsonObject jsonObject;
		ObjectManagerInfo::s_objects[i]->WriteToFile(jsonObject);
		json_manager.WriteArrayPush("objects", jsonObject);
	}
}

void CS_ObjectManager::AddArcheType(CS_ArcheTypes type, const char* jsonPath)
{
	if (ObjectManagerInfo::s_archetypes.find(type) != ObjectManagerInfo::s_archetypes.end())
		return;
	CS_JsonManager json_manager;
	json_manager.OpenFile(jsonPath);
	CS_Object* pObj = new CS_Object(type);
	pObj->ReadFromFile(json_manager);

	std::stringstream ss;
	json_manager.ReadData("ComponentsToBeAdded", ss);
	std::string Component;
	while (ss >> Component)
	{
            CS_Component*p_component =
            ObjectManagerInfo::s_componentFactory.BuildComponentFromFactory(StringToComponent(Component));
            CS_JsonObject File;
            json_manager.GetObj(Component.c_str(), File);
            p_component->ReadFromFile(File);
            pObj->AddComponent(p_component);
	}
        ObjectManagerInfo::s_archetypes.insert(std::make_pair(type, pObj));
}

void CS_ObjectManager::AddObject(CS_Object * objToAdd)
{
    auto found = std::find(ObjectManagerInfo::s_objects.begin(), ObjectManagerInfo::s_objects.end(), objToAdd);
    if (found != ObjectManagerInfo::s_objects.end())
    	return;
    
    ObjectManagerInfo::s_objects.push_back(objToAdd);
}

CS_Object * CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes type)
{
    auto found = ObjectManagerInfo::s_archetypes.find(type);
    if (found == ObjectManagerInfo::s_archetypes.end())
        return nullptr;

    CS_Object* pClone = found->second->Clone();
    ObjectManagerInfo::s_objects.push_back(pClone);
    return pClone;
}

std::vector<CS_Object*> CS_ObjectManager::CreateParticles(int amount, CS_ArcheTypes type)
{
	std::vector<CS_Object*> vec;
	auto found = ObjectManagerInfo::s_archetypes.find(type);
	if (found == ObjectManagerInfo::s_archetypes.end())
		return vec;
	for (int i = 0; i < amount; i++) {
		CS_Object* pObject = found->second->Clone();
		vec.push_back(pObject);
	}
	return vec;
}

void CS_ObjectManager::DestoryObject(CS_Object * objToDel)
{
    //Make sure the object exists
    auto found = std::find(ObjectManagerInfo::s_objects.begin() + ObjectManagerInfo::s_objectStart, ObjectManagerInfo::s_objects.end(), objToDel);
    if (found == ObjectManagerInfo::s_objects.end())
        return;

    (*found)->is_dead = true;
}

void CS_ObjectManager::DestroyAllObjects(bool destroyPaused)
{
    if (destroyPaused)
    {
        int size = static_cast<int>(ObjectManagerInfo::s_objects.size());
        for (int i = 0; i < size; ++i)
        {
            delete ObjectManagerInfo::s_objects[i];
            ObjectManagerInfo::s_objects[i] = nullptr;
        }
		CS_Physics::RemoveAllColliderComponents();
        ObjectManagerInfo::s_objects.clear();
    }
    else
    {
        int size = static_cast<int>(ObjectManagerInfo::s_objects.size());
        for (int i = size - 1; i >= ObjectManagerInfo::s_objectStart; --i)
        {
            delete ObjectManagerInfo::s_objects[i];
            ObjectManagerInfo::s_objects[i] = nullptr;
            ObjectManagerInfo::s_objects.pop_back();
        }
    }
}

CS_Component * CS_ObjectManager::CreateComponentWithComponentType(CS_ComponentTypes type)
{
	return ObjectManagerInfo::s_componentFactory.BuildComponentFromFactory(type);
}

void CS_ObjectManager::AddComponent(CS_ComponentTypes type, CS_ComponentBuilder * pBuilder)
{
    ObjectManagerInfo::s_componentFactory.AddBuilderIntoFactory(type, pBuilder);
}

CS_Object * CS_ObjectManager::GetMouseInObj()
{
    int size = static_cast<int>(ObjectManagerInfo::s_objects.size());
    for (int i = ObjectManagerInfo::s_objectStart; i < size; ++i)
    {
        if (ObjectManagerInfo::s_objects[i]->editMouseSelectAABB.IsMouseIn())
            return ObjectManagerInfo::s_objects[i];
    }
    return nullptr;
}

CS_Object * CS_ObjectManager::GetFirstObjectByType(CS_ArcheTypes type)
{
	CS_Object *pObj = nullptr;
	for (size_t i = ObjectManagerInfo::s_objectStart; i < ObjectManagerInfo::s_objects.size(); ++i)
	{
		if (ObjectManagerInfo::s_objects[i]->archetype == type)
		{
			pObj = ObjectManagerInfo::s_objects[i];
			return pObj;
		}
	}
	return pObj;
}

CS_Object * CS_ObjectManager::GetFirstObjectByName(const std::string & name)
{
	CS_Object *pObj = nullptr;
	for (size_t i = ObjectManagerInfo::s_objectStart; i < ObjectManagerInfo::s_objects.size(); ++i)
	{
		if (ObjectManagerInfo::s_objects[i]->m_name == name)
		{
			pObj = ObjectManagerInfo::s_objects[i];
			return pObj;
		}
	}
	return pObj;
}

void CS_ObjectManager::Init()
{
    ObjectManagerInfo::s_objects.reserve(100);//start size
    ObjectManagerInfo::s_objectStart = 0;
	CS_AddComponents();
	CS_AddArcheTypes();
}

void CS_ObjectManager::Update(float dt)
{
    for (size_t i = ObjectManagerInfo::s_objectStart; i < ObjectManagerInfo::s_objects.size(); ++i)
    {
    	if (ObjectManagerInfo::s_objects[i]->is_dead)
    	{
    		delete ObjectManagerInfo::s_objects[i];
    		ObjectManagerInfo::s_objects[i] = ObjectManagerInfo::s_objects[ObjectManagerInfo::s_objects.size() - 1];
			ObjectManagerInfo::s_objects.pop_back();
    		--i;//so that we can update the shifted object 
    	}
    	else
    	{
    		ObjectManagerInfo::s_objects[i]->Update(dt);
		}
    }
    ObjectManagerInfo::particleGen.Update(dt);
}

void CS_ObjectManager::Pause()
{
    ObjectManagerInfo::s_pauseStack.push(ObjectManagerInfo::s_objectStart);
    ObjectManagerInfo::s_objectStart = static_cast<int>(ObjectManagerInfo::s_objects.size());
}

void CS_ObjectManager::Resume()
{
    ObjectManagerInfo::s_objectStart = ObjectManagerInfo::s_pauseStack.top();
    ObjectManagerInfo::s_pauseStack.pop();
}
