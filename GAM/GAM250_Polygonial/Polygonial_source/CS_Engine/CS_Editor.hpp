/**
 * @file CS_Editor.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Code for editor
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include <string>


class CS_Camera;

class CS_Editor
{
public:
	static bool IsEditModeOn();
	static void EditStart(CS_Camera* cam);
    static void Update(float dt);
	static void EditEnd();
	static std::string GetSelectedLevel();
	static void UnSelectObject();
        static void SaveArchetype();
private:
	static void SaveAll();
        
    static void CameraControl(float dt);
    static void SelectObject();
	static void ShowMenuBar();
	static void SetPositionEditWindow();
        static void HighLightSelectedObject(float dt);
	//debug settings{
	static void ShowDebugSettings();
	//}
	//spawn menu{
	static void ShowSpawnButton();
	static void ShowSpawnMenu();
	//}
	//Edit menu{
	static void ShowAddComponentButton();
	static void ShowAddComponentTree();
	static void ShowObjectEditMenu();
	//}

    static void ShowGui();
	
	
};
