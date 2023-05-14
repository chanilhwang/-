#pragma once 
#include <string> 
 
 
enum class CS_ComponentTypes {  
AABB_BoxCollider_Component, 
Graphics_Component, 
OBB_BoxCollider_Component, 
Physics_Component, 
SphereCollider_Component, 
Transform_Component, 
Background_Component, 
Background_NO_Adv_Component, 
Bomb_Component, 
Bullet_Component, 
Chase_Component, 
EnemyGenerator_Component, 
Enemy_Component, 
GenEffect_Component, 
Item_Component, 
Player_Component, 
Shoot_Component, 
Simple_Map_Component, 
Strafe_Component, 
INVALID_Component 
}; 
 
 
inline CS_ComponentTypes StringToComponent(const std::string& string) { 
if(string == "AABB_BoxCollider_Component") return CS_ComponentTypes::AABB_BoxCollider_Component; 
if(string == "Graphics_Component") return CS_ComponentTypes::Graphics_Component; 
if(string == "OBB_BoxCollider_Component") return CS_ComponentTypes::OBB_BoxCollider_Component; 
if(string == "Physics_Component") return CS_ComponentTypes::Physics_Component; 
if(string == "SphereCollider_Component") return CS_ComponentTypes::SphereCollider_Component; 
if(string == "Transform_Component") return CS_ComponentTypes::Transform_Component; 
if(string == "Background_Component") return CS_ComponentTypes::Background_Component; 
if(string == "Background_NO_Adv_Component") return CS_ComponentTypes::Background_NO_Adv_Component; 
if(string == "Bomb_Component") return CS_ComponentTypes::Bomb_Component; 
if(string == "Bullet_Component") return CS_ComponentTypes::Bullet_Component; 
if(string == "Chase_Component") return CS_ComponentTypes::Chase_Component; 
if(string == "EnemyGenerator_Component") return CS_ComponentTypes::EnemyGenerator_Component; 
if(string == "Enemy_Component") return CS_ComponentTypes::Enemy_Component; 
if(string == "GenEffect_Component") return CS_ComponentTypes::GenEffect_Component; 
if(string == "Item_Component") return CS_ComponentTypes::Item_Component; 
if(string == "Player_Component") return CS_ComponentTypes::Player_Component; 
if(string == "Shoot_Component") return CS_ComponentTypes::Shoot_Component; 
if(string == "Simple_Map_Component") return CS_ComponentTypes::Simple_Map_Component; 
if(string == "Strafe_Component") return CS_ComponentTypes::Strafe_Component; 
return CS_ComponentTypes::INVALID_Component; 
} 
inline std::string ComponentToString(const CS_ComponentTypes type) { 
if(type == CS_ComponentTypes::AABB_BoxCollider_Component) return "AABB_BoxCollider_Component"; 
if(type == CS_ComponentTypes::Graphics_Component) return "Graphics_Component"; 
if(type == CS_ComponentTypes::OBB_BoxCollider_Component) return "OBB_BoxCollider_Component"; 
if(type == CS_ComponentTypes::Physics_Component) return "Physics_Component"; 
if(type == CS_ComponentTypes::SphereCollider_Component) return "SphereCollider_Component"; 
if(type == CS_ComponentTypes::Transform_Component) return "Transform_Component"; 
if(type == CS_ComponentTypes::Background_Component) return "Background_Component"; 
if(type == CS_ComponentTypes::Background_NO_Adv_Component) return "Background_NO_Adv_Component"; 
if(type == CS_ComponentTypes::Bomb_Component) return "Bomb_Component"; 
if(type == CS_ComponentTypes::Bullet_Component) return "Bullet_Component"; 
if(type == CS_ComponentTypes::Chase_Component) return "Chase_Component"; 
if(type == CS_ComponentTypes::EnemyGenerator_Component) return "EnemyGenerator_Component"; 
if(type == CS_ComponentTypes::Enemy_Component) return "Enemy_Component"; 
if(type == CS_ComponentTypes::GenEffect_Component) return "GenEffect_Component"; 
if(type == CS_ComponentTypes::Item_Component) return "Item_Component"; 
if(type == CS_ComponentTypes::Player_Component) return "Player_Component"; 
if(type == CS_ComponentTypes::Shoot_Component) return "Shoot_Component"; 
if(type == CS_ComponentTypes::Simple_Map_Component) return "Simple_Map_Component"; 
if(type == CS_ComponentTypes::Strafe_Component) return "Strafe_Component"; 
return "INVALID_Component"; 
} 
