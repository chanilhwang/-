/**
 * @file CS_Object.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Basic object data initialization.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_ComponentTypes.hpp"
#include "Transform_Component.hpp"
#include "Graphics_Component.hpp"
#include <vector>
#include <array>
#include "Physics_Component.hpp"
#include "CS_ArcheTypes.hpp"
#include "CS_EditMouseSelectAABB.hpp"
class CS_JsonManager;
class CS_Component;
class CS_Object
{
public:
    CS_Object(CS_ArcheTypes type=CS_ArcheTypes::INVALID_ArcheType);
    ~CS_Object();
	void ReadFromFile(CS_JsonManager& jsonManager);
	void WriteToFile(CS_JsonObject& jsonObject);
    bool AddComponent(CS_Component* pComponent);
    void RemoveAllComponents();
    template<typename T>
    void GetComponent(CS_ComponentTypes componentType, T*& pComp);
    void Update(float dt);
    CS_Object* Clone(void);
	void ShowGui();
	//i will not gonna add gfx transform and physics on components vector
    bool is_inside_screen(void) const;
    bool is_inside_screen_enemy(void) const;
    bool is_outside_screen(void) const;
	void trigger_screenshake(void) const;
    Transform_Component transform;
    Graphics_Component gfx;
    Physics_Component physics;
    CS_EditMouseSelectAABB editMouseSelectAABB;
    bool is_dead;
    const int my_id;
	mutable  CS_ArcheTypes archetype;
	std::string m_name;
        std::array<char, 30> m_gui_name= {0, };
            
private:
    typedef std::vector<CS_Component*> Components;
    Components m_components;
    static int s_objectIDCounter;
    
};

template<typename T>
inline void CS_Object::GetComponent(CS_ComponentTypes componentType, T *& pComp)
{
	if(componentType== CS_ComponentTypes::INVALID_Component)
	{
		pComp = nullptr;
		return;
	}
    if(componentType== CS_ComponentTypes::Transform_Component)
    {
        pComp = reinterpret_cast<T*>(&transform);
        return;
    }
	if (componentType == CS_ComponentTypes::Physics_Component)
	{
		pComp = reinterpret_cast<T*>(&physics);
		return;
	}
	if (componentType == CS_ComponentTypes::Graphics_Component)
	{
		pComp = reinterpret_cast<T*>(&gfx);
		return;
	}
    for (auto component:m_components)
    {
        if(component->GetComponentType() == componentType)
        {
            pComp = reinterpret_cast<T*>(component);
            return;
        }
    }
    pComp = nullptr;
}
