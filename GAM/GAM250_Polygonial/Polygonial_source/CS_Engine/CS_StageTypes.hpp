#pragma once 
#include <string> 
 
 
enum class CS_StageTypes {  
Before_GamePlay_Stage, 
Clear_Stage, 
Credit_Stage, 
Edit_Stage, 
GamePlay1_Stage, 
Lose_Stage, 
Main_Menu_Stage, 
Menu_Stage, 
Quit_Check_Stage, 
Splash_Stage, 
Test_Stage, 
INVALID_Stage 
}; 
 
 
inline CS_StageTypes StringToStage(const std::string& string) { 
if(string == "Before_GamePlay_Stage") return CS_StageTypes::Before_GamePlay_Stage; 
if(string == "Clear_Stage") return CS_StageTypes::Clear_Stage; 
if(string == "Credit_Stage") return CS_StageTypes::Credit_Stage; 
if(string == "Edit_Stage") return CS_StageTypes::Edit_Stage; 
if(string == "GamePlay1_Stage") return CS_StageTypes::GamePlay1_Stage; 
if(string == "Lose_Stage") return CS_StageTypes::Lose_Stage; 
if(string == "Main_Menu_Stage") return CS_StageTypes::Main_Menu_Stage; 
if(string == "Menu_Stage") return CS_StageTypes::Menu_Stage; 
if(string == "Quit_Check_Stage") return CS_StageTypes::Quit_Check_Stage; 
if(string == "Splash_Stage") return CS_StageTypes::Splash_Stage; 
if(string == "Test_Stage") return CS_StageTypes::Test_Stage; 
return CS_StageTypes::INVALID_Stage; 
} 
