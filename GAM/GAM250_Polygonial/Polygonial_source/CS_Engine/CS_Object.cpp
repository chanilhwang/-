/**
 * @file CS_Object.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Basic object data initialization.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Object.hpp"
#include "CS_Components.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_ArcheTypes.hpp"
#include "CS_JsonManager.hpp"
#include "imgui.h"
#include "CS_StageManager.hpp"
#include <iostream>
#include "CS_Input.hpp"
#include "CS_Editor.hpp"
#include <array>
#include "CS_ColliderComponent.hpp"
int CS_Object::s_objectIDCounter = 0;

CS_Object::CS_Object(CS_ArcheTypes type):is_dead(false),my_id(++s_objectIDCounter),archetype(type)
{
	m_name = "object";
	m_name += std::to_string(my_id);
	transform.SetParent(this);
	gfx.SetParent(this);
	physics.SetParent(this);
    editMouseSelectAABB.SetParent(this);
    std::copy(std::begin(m_name), std::end(m_name), std::begin(m_gui_name));
    //CS_ObjectManager::AddObject(this);
}


CS_Object::~CS_Object()
{
    RemoveAllComponents();
}
//handle basic reading
void CS_Object::ReadFromFile(CS_JsonManager& jsonManager)
{
	jsonManager.ReadData("Name", m_name);
	jsonManager.ReadData("Archetype", archetype);
    CS_JsonObject Transform;
    jsonManager.GetObj("Transform_Component", Transform);
    transform.ReadFromFile(Transform);
    CS_JsonObject Gfx;
    jsonManager.GetObj("Graphics_Component", Gfx);
    gfx.ReadFromFile(Gfx);
    CS_JsonObject Physic;
    jsonManager.GetObj("Physics_Component", Physic);
    physics.ReadFromFile(Physic);
}

void CS_Object::WriteToFile(CS_JsonObject& jsonObject)
{
	jsonObject.WriteData("Name", m_name);
	jsonObject.WriteData("Archetype", archetype);
	CS_JsonObject Transform;
	transform.WriteToFile(Transform);
	jsonObject.WriteObejct("Transform_Component", Transform);
	CS_JsonObject Gfx;
	gfx.WriteToFile(Gfx);
	jsonObject.WriteObejct("Graphics_Component", Gfx);
	CS_JsonObject Physic;
	physics.WriteToFile(Physic);
	jsonObject.WriteObejct("Physics_Component", Physic);
	size_t size = m_components.size();
	for (auto& comp:m_components)
	{
		CS_JsonObject j_object;
		comp->WriteToFile(j_object);
		auto key = ComponentToString(comp->GetComponentType());
		jsonObject.WriteArrayPush("ComponentsToBeAdded", key);
		jsonObject.WriteObejct(key, j_object);
	}
}

bool CS_Object::AddComponent(CS_Component * pComponent)
{
	if (pComponent->GetComponentType() == CS_ComponentTypes::INVALID_Component)
		return false;
	if (pComponent->GetComponentType() == CS_ComponentTypes::Transform_Component)
		return false;
	if (pComponent->GetComponentType() == CS_ComponentTypes::Physics_Component)
		return false;
	if (pComponent->GetComponentType() == CS_ComponentTypes::Graphics_Component)
		return false;
    //check there is same component
    auto found = std::find(m_components.begin(), m_components.end(), pComponent);
    if (found != m_components.end())
        return false;
	//do not add if it is not allowed to have same types
	if(pComponent->ShouldBeOnlyOneInOneObject())
	{
		for(auto comp:m_components)
			if(comp->GetComponentType()==pComponent->GetComponentType())
				return false;
	}

    //Set this object as the parent
    pComponent->SetParent(this);
	pComponent->Init();
    m_components.push_back(pComponent);
	return true;
}

void CS_Object::RemoveAllComponents()
{
    auto itor = m_components.begin();
    auto end = m_components.end();
    while (itor != end)
    {
        delete (*itor);
        ++itor;
    }
    m_components.clear();
}

void CS_Object::Update(float dt)
{
    //for now just simple test i should change this start from back since it is vector when i delete
    //it is better to pop but for now we don't have any dead compo
	if (!CS_Editor::IsEditModeOn())
	{
		transform.Update(dt);
		gfx.Update(dt);
		physics.Update(dt);

		editMouseSelectAABB.Update();
		int endIndex = static_cast<int>(m_components.size()) - 1;
		for (; endIndex >= 0; --endIndex)
		{
			if (m_components[endIndex]->is_Dead)
			{
				delete m_components[endIndex];
				m_components[endIndex] = m_components[m_components.size() - 1];
				m_components.pop_back();
			}
			else
			{
				m_components[endIndex]->Update(dt);
			}
		}
	}
	else
	{
		transform.Update(dt);
		gfx.Update(dt);
		editMouseSelectAABB.Update();
		int endIndex = static_cast<int>(m_components.size()) - 1;
		for (; endIndex >= 0; --endIndex)
		{
			if (m_components[endIndex]->is_Dead)
			{
				delete m_components[endIndex];
				m_components[endIndex] = m_components[m_components.size() - 1];
				m_components.pop_back();
			}
		}
	}
}

CS_Object * CS_Object::Clone(void)
{
    //create object
    CS_Object* pClone = new CS_Object(archetype);
    //copy this internal data
    pClone->transform=transform;
    pClone->physics=physics;
    pClone->gfx = gfx;
    
    //clone all components
    size_t size = m_components.size();
    for (size_t i = 0; i < size; ++i)
    {
        CS_Component* pComp = m_components[i]->Clone();
        pClone->AddComponent(pComp);
    }
    return pClone;
}

void CS_Object::ShowGui()
{
	if (ImGui::TreeNode("Object INFO"))
	{
                if(m_gui_name.data())
		    ImGui::InputText("Name", m_gui_name.data(), m_gui_name.size());
                if (!std::equal(std::begin(m_name), std::end(m_name), std::begin(m_gui_name)))
                {
                    m_name.clear();
                    for (char element : m_gui_name)
                    {
                        m_name.push_back(element);
                    }
                }
                if (ImGui::Button("Export"))
                    CS_Editor::SaveArchetype();
		is_dead = ImGui::Button("Delete");
		if (is_dead)
			CS_Editor::UnSelectObject();
		ImGui::TreePop();
	}
	if(ImGui::TreeNode("Current Components"))
	{
		transform.ShowGui();
		physics.ShowGui();
		gfx.ShowGui();
		for (auto compo : m_components)
		{
			compo->ShowGui();
		}
		ImGui::TreePop();
	}
}

bool CS_Object::is_inside_screen() const
{
    bool four_side_check[4];
    four_side_check[0] = transform.GetTranslation().x <= CS_StageManager::GetCameraOfCurrentLevel()->GetRight_x();
    four_side_check[1] = transform.GetTranslation().x >= CS_StageManager::GetCameraOfCurrentLevel()->GetLeft_x();
    four_side_check[2] = transform.GetTranslation().y <= CS_StageManager::GetCameraOfCurrentLevel()->GetTop_y();
    four_side_check[3] = transform.GetTranslation().y >= CS_StageManager::GetCameraOfCurrentLevel()->GetBot_y();
    for(int i = 0; i<4; i++)
    {
        if (!four_side_check[i])
            return false;
    }
    return true;
}


bool CS_Object::is_inside_screen_enemy() const
{
    if (transform.GetTranslation().x <= CS_StageManager::GetCameraOfCurrentLevel()->GetRight_x())
        return true;
    return false;
}

bool CS_Object::is_outside_screen() const
{
    if (transform.GetTranslation().x <= CS_StageManager::GetCameraOfCurrentLevel()->GetLeft_x())
    {
        return true;
    }
    return false;
}

void CS_Object::trigger_screenshake(void) const
{
	CS_StageManager::GetCameraOfCurrentLevel()->SetShakeOn();
}
