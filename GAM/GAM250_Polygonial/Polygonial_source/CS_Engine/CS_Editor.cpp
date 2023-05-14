/**
 * @file CS_Editor.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Code for editor
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Editor.hpp"
#include "CS_Camera.hpp"
#include "CS_Input.hpp"
#include "CS_Object.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Graphics.hpp"
#include "imgui.h"
#include "CS_ResourceLists.hpp"
#include "CS_JsonManager.hpp"
#include "CS_StageManager.hpp"
#include <iostream>

namespace EditorInfo
{
    bool IsEditorOn = false;
    CS_Object* selectedObject = nullptr;
    CS_Camera* cam = nullptr;
	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize 
	| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	CS_JsonManager json_manager;
	
    float camMovementSpeed=200;
	std::string selectedArcheType = "";
	std::string selectedComponent = "";
	std::string selectedLevel = "new_level.json";
	std::string toSaveLevel = "new_level.json";
	bool is_removing = false;
}
bool CS_Editor::IsEditModeOn()
{
	return EditorInfo::IsEditorOn;
}
void CS_Editor::EditStart(CS_Camera* cam)
{
    EditorInfo::IsEditorOn = true;
    EditorInfo::selectedObject = nullptr;
    EditorInfo::cam = cam;
	EditorInfo::selectedArcheType = "";
	EditorInfo::selectedComponent = "";
	EditorInfo::json_manager.OpenFile(EditorInfo::selectedLevel);
	EditorInfo::json_manager.RemoveAll();
	EditorInfo::is_removing = false;
}

void CS_Editor::Update(float dt)
{
    CameraControl(dt);
    SelectObject();
    HighLightSelectedObject(dt);
    ShowGui();
	bool ctrlandS = CS_Input::IsPressed(InputButtons::Ctrl) && CS_Input::IsPressed(InputButtons::S);
	bool ctrlandR = CS_Input::IsPressed(InputButtons::Ctrl) && CS_Input::IsPressed(InputButtons::R);
	if(ctrlandS)
	{
		//EditorInfo::json_manager.RemoveAll();
		//CS_ObjectManager::WriteObejctToFile(EditorInfo::json_manager);
		//EditorInfo::json_manager.Write("new_level.json");
		SaveAll();
	}
	if(ctrlandR)
	{
		EditorInfo::selectedLevel = "new_level.json";
		EditorInfo::toSaveLevel = "new_level.json";
		EditorInfo::is_removing = true;
		CS_StageManager::Restart();
	}
	
}

void CS_Editor::EditEnd()
{
	//EditorInfo::json_manager.RemoveAll();
	//CS_ObjectManager::WriteObejctToFile(EditorInfo::json_manager);
	//EditorInfo::json_manager.Write("new_level.json");
	SaveAll();

    EditorInfo::IsEditorOn = false;
    EditorInfo::selectedObject = nullptr;
    EditorInfo::cam = nullptr;
}

void CS_Editor::CameraControl(float dt)
{
	bool ctrlkey = CS_Input::IsPressed(InputButtons::Ctrl);
	
	if (!ctrlkey)
	{
		if (CS_Input::IsPressed(InputButtons::W))
			EditorInfo::cam->MoveUp(EditorInfo::camMovementSpeed*dt);
		if (CS_Input::IsPressed(InputButtons::A))
			EditorInfo::cam->MoveRight(-EditorInfo::camMovementSpeed *dt);
		if (CS_Input::IsPressed(InputButtons::S))
			EditorInfo::cam->MoveUp(-EditorInfo::camMovementSpeed * dt);
		if (CS_Input::IsPressed(InputButtons::D))
			EditorInfo::cam->MoveRight(EditorInfo::camMovementSpeed*dt);
		if (CS_Input::IsReleased(InputButtons::Mouse_Middle))
			EditorInfo::cam->SetZoom(1.f);
	}
	if (ctrlkey)
	{
		float new_zoom = EditorInfo::cam->GetZoom() + (CS_Input::GetMouseWheelScrollAmount() * 0.001f);
		if (new_zoom > 4.0)
			new_zoom = 4.f;
		if (new_zoom < 0.1f)
			new_zoom = 0.1f;
		EditorInfo::cam->SetZoom(new_zoom);
	}
}

void CS_Editor::SelectObject()
{
    if (CS_Input::IsPressed(InputButtons::Mouse_Right))
    {
        EditorInfo::selectedObject = CS_ObjectManager::GetMouseInObj();
        if (EditorInfo::selectedObject)
            EditorInfo::selectedObject->transform.SetTranslation(CS_Input::GetMouseWorldPos());
    }

}

void CS_Editor::HighLightSelectedObject(float dt)
{
    if (EditorInfo::selectedObject)
        EditorInfo::selectedObject->transform.SetRotation(EditorInfo::selectedObject->transform.GetRotation() + dt * 10.f);
}
void CS_Editor::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("new_level","Ctrl+R"))
			{
				EditorInfo::selectedLevel = "new_level.json";
				EditorInfo::toSaveLevel = "new_level.json";
				EditorInfo::is_removing = true;
				CS_StageManager::Restart();
			}
			if(ImGui::MenuItem("Save", "Ctrl+S"))
			{
				SaveAll();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void CS_Editor::SetPositionEditWindow()
{
	ImGui::SetNextWindowPos({ CS_Graphics::GetDisplayAreaSize().x,0 }, 0, { 1,0 });
	ImGui::Begin("Editor", nullptr, EditorInfo::flags);
	ImGui::End();
}

void CS_Editor::ShowDebugSettings()
{
	ImGui::Begin("Editor", nullptr, EditorInfo::flags);
	if (ImGui::CollapsingHeader("Debug Settings"))
	{
		ImGui::DragFloat("DebugCamMoveSpeed", &EditorInfo::camMovementSpeed);
	}
	ImGui::End();
}

void CS_Editor::ShowSpawnButton()
{
	auto io = ImGui::GetIO();
	static bool ObjectWasMade = false;
	ImGui::Button("Create Object(Drag Me)");
	if (ImGui::IsItemActive())
	{
		// Draw a line between the button and the mouse cursor
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->PushClipRectFullScreen();
		draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
		draw_list->PopClipRect();
		if (!ObjectWasMade)
		{
			EditorInfo::selectedObject = CS_ObjectManager::CreateObjectWithArcheType(StringToArcheType(EditorInfo::selectedArcheType));
			EditorInfo::selectedObject->transform.SetTranslation(CS_Input::GetMouseWorldPos());
			ObjectWasMade = true;
		}
		else
		{
			//objectfollow mouse
			EditorInfo::selectedObject->transform.SetTranslation(CS_Input::GetMouseWorldPos());
		}
	}
	else
		ObjectWasMade = false;
}

void CS_Editor::ShowSpawnMenu()
{
	ImGui::Begin("Editor", nullptr, EditorInfo::flags);
	if (ImGui::CollapsingHeader("Spawn Menu"))
	{
		static int selected = 0;
		ImGui::ListBox("selectArch", &selected, CS_ArcheTypeList, CS_ArcheTypeListSize, -1);
		EditorInfo::selectedArcheType = CS_ArcheTypeList[selected];
		ShowSpawnButton();
	}
	
	ImGui::End();
}

void CS_Editor::ShowAddComponentButton()
{
	if (ImGui::Button("Add"))
	{
		CS_Component*p_component =
			CS_ObjectManager::CreateComponentWithComponentType(StringToComponent(EditorInfo::selectedComponent));
		bool NoSameComp=EditorInfo::selectedObject->AddComponent(p_component);
		if (!NoSameComp)
		{
			//std::cout << "there is same\n";
			delete p_component;
		}
	}
}

void CS_Editor::ShowAddComponentTree()
{
	if (ImGui::TreeNode("Add Component"))
	{
		static int selected = 0;
		ImGui::ListBox("selectArch", &selected, CS_ComponentTypeList, CS_ComponentTypeListSize, -1);
		EditorInfo::selectedComponent = CS_ComponentTypeList[selected];
		ShowAddComponentButton();
		ImGui::TreePop();
	}
}

void CS_Editor::ShowObjectEditMenu()
{
	ImGui::Begin("Object Edit Menu", nullptr, EditorInfo::flags);
	if (ImGui::CollapsingHeader("Object Edit Menu"))
	{
		if (EditorInfo::selectedObject == nullptr)
			ImGui::Text("Please select object to edit");
		else
		{
			ShowAddComponentTree();
			EditorInfo::selectedObject->ShowGui();//show tree node of comps
		}
	}
	ImGui::End();
}

void CS_Editor::ShowGui()
{
	ShowMenuBar();
    
	SetPositionEditWindow();
	ShowDebugSettings();
	ShowSpawnMenu();
	ShowObjectEditMenu();
}


std::string CS_Editor::GetSelectedLevel()
{
	return EditorInfo::selectedLevel;
}

void CS_Editor::UnSelectObject()
{
	EditorInfo::selectedObject = nullptr;
}

void CS_Editor::SaveAll()
{
	if(EditorInfo::is_removing)
	{
		EditorInfo::json_manager.RemoveAll();
		EditorInfo::json_manager.Write("new_level.json");
	}
	else
	{
		EditorInfo::json_manager.RemoveAll();
		CS_ObjectManager::WriteObejctToFile(EditorInfo::json_manager);
		EditorInfo::json_manager.Write("new_level.json");
	}
}

void CS_Editor::SaveArchetype()
{
    CS_JsonObject jsonObject;
    EditorInfo::selectedObject->WriteToFile(jsonObject);
    EditorInfo::json_manager.WriteArrayPush("objects", jsonObject);
    std::string total_name1 = EditorInfo::selectedObject->m_name;
    std::string total_name2;
    int str_pos = 0;
    while(total_name1[str_pos]!='\0')
    {
        total_name2.push_back(total_name1[str_pos]);
        ++str_pos;
    }
    EditorInfo::json_manager.Write(total_name2 + ".json");
}
