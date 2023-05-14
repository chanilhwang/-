#include "CS_AddComponents.hpp" 
#include "CS_ObjectManager.hpp" 
#include "CS_ComponentTypes.hpp" 
#include "CS_ComponentBuilder.hpp" 
#include "AABB_BoxCollider_Component.hpp" 
#include "Graphics_Component.hpp" 
#include "OBB_BoxCollider_Component.hpp" 
#include "Physics_Component.hpp" 
#include "SphereCollider_Component.hpp" 
#include "Transform_Component.hpp" 
#include "Background_Component.hpp" 
#include "Background_NO_Adv_Component.hpp" 
#include "Bomb_Component.hpp" 
#include "Bullet_Component.hpp" 
#include "Chase_Component.hpp" 
#include "EnemyGenerator_Component.hpp" 
#include "Enemy_Component.hpp" 
#include "GenEffect_Component.hpp" 
#include "Item_Component.hpp" 
#include "Player_Component.hpp" 
#include "Shoot_Component.hpp" 
#include "Simple_Map_Component.hpp" 
#include "Strafe_Component.hpp" 
 
 
void CS_AddComponents(void) {  
CS_ObjectManager::AddComponent(CS_ComponentTypes::AABB_BoxCollider_Component, new CS_TemplatedComponentBuilder< AABB_BoxCollider_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Graphics_Component, new CS_TemplatedComponentBuilder< Graphics_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::OBB_BoxCollider_Component, new CS_TemplatedComponentBuilder< OBB_BoxCollider_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Physics_Component, new CS_TemplatedComponentBuilder< Physics_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::SphereCollider_Component, new CS_TemplatedComponentBuilder< SphereCollider_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Transform_Component, new CS_TemplatedComponentBuilder< Transform_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Background_Component, new CS_TemplatedComponentBuilder< Background_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Background_NO_Adv_Component, new CS_TemplatedComponentBuilder< Background_NO_Adv_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Bomb_Component, new CS_TemplatedComponentBuilder< Bomb_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Bullet_Component, new CS_TemplatedComponentBuilder< Bullet_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Chase_Component, new CS_TemplatedComponentBuilder< Chase_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::EnemyGenerator_Component, new CS_TemplatedComponentBuilder< EnemyGenerator_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Enemy_Component, new CS_TemplatedComponentBuilder< Enemy_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::GenEffect_Component, new CS_TemplatedComponentBuilder< GenEffect_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Item_Component, new CS_TemplatedComponentBuilder< Item_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Player_Component, new CS_TemplatedComponentBuilder< Player_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Shoot_Component, new CS_TemplatedComponentBuilder< Shoot_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Simple_Map_Component, new CS_TemplatedComponentBuilder< Simple_Map_Component >() ); 
CS_ObjectManager::AddComponent(CS_ComponentTypes::Strafe_Component, new CS_TemplatedComponentBuilder< Strafe_Component >() ); 
} 
