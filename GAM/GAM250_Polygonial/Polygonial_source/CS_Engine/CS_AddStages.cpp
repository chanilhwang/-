#include "CS_AddStages.hpp" 
#include "CS_StageManager.hpp" 
#include "CS_StageTypes.hpp" 
#include "CS_StageBuilder.hpp" 
#include "Before_GamePlay_Stage.hpp" 
#include "Clear_Stage.hpp" 
#include "Credit_Stage.hpp" 
#include "Edit_Stage.hpp" 
#include "GamePlay1_Stage.hpp" 
#include "Lose_Stage.hpp" 
#include "Main_Menu_Stage.hpp" 
#include "Menu_Stage.hpp" 
#include "Quit_Check_Stage.hpp" 
#include "Splash_Stage.hpp" 
#include "Test_Stage.hpp" 
 
 
void CS_AddStages(void) {  
CS_StageManager::AddStage(CS_StageTypes::Before_GamePlay_Stage, new CS_TemplatedStageBuilder< Before_GamePlay_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Clear_Stage, new CS_TemplatedStageBuilder< Clear_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Credit_Stage, new CS_TemplatedStageBuilder< Credit_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Edit_Stage, new CS_TemplatedStageBuilder< Edit_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::GamePlay1_Stage, new CS_TemplatedStageBuilder< GamePlay1_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Lose_Stage, new CS_TemplatedStageBuilder< Lose_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Main_Menu_Stage, new CS_TemplatedStageBuilder< Main_Menu_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Menu_Stage, new CS_TemplatedStageBuilder< Menu_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Quit_Check_Stage, new CS_TemplatedStageBuilder< Quit_Check_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Splash_Stage, new CS_TemplatedStageBuilder< Splash_Stage >() ); 
CS_StageManager::AddStage(CS_StageTypes::Test_Stage, new CS_TemplatedStageBuilder< Test_Stage >() ); 
} 
