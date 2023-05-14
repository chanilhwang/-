#pragma once 
#include <string> 
 
 
enum class CS_ArcheTypes {  
Barrel_Item, 
Black_Bullet, 
Black_Particle, 
Blue_Particle, 
Bubble, 
Bullet, 
ChasingEnemy, 
Chasing_Enemy, 
DMG_Item, 
Enemy, 
GenEffect, 
HP_Item, 
Map, 
NewObject, 
Particle, 
Player, 
ShootingEnemy, 
Shooting_Enemy, 
ShotRate_Item, 
Strafing_Enemy, 
White_Bullet, 
White_Particle, 
Yellow_Particle, 
INVALID_ArcheType 
}; 
 
 
inline CS_ArcheTypes StringToArcheType(const std::string& string) { 
if(string == "Barrel_Item") return CS_ArcheTypes::Barrel_Item; 
if(string == "Black_Bullet") return CS_ArcheTypes::Black_Bullet; 
if(string == "Black_Particle") return CS_ArcheTypes::Black_Particle; 
if(string == "Blue_Particle") return CS_ArcheTypes::Blue_Particle; 
if(string == "Bubble") return CS_ArcheTypes::Bubble; 
if(string == "Bullet") return CS_ArcheTypes::Bullet; 
if(string == "ChasingEnemy") return CS_ArcheTypes::ChasingEnemy; 
if(string == "Chasing_Enemy") return CS_ArcheTypes::Chasing_Enemy; 
if(string == "DMG_Item") return CS_ArcheTypes::DMG_Item; 
if(string == "Enemy") return CS_ArcheTypes::Enemy; 
if(string == "GenEffect") return CS_ArcheTypes::GenEffect; 
if(string == "HP_Item") return CS_ArcheTypes::HP_Item; 
if(string == "Map") return CS_ArcheTypes::Map; 
if(string == "NewObject") return CS_ArcheTypes::NewObject; 
if(string == "Particle") return CS_ArcheTypes::Particle; 
if(string == "Player") return CS_ArcheTypes::Player; 
if(string == "ShootingEnemy") return CS_ArcheTypes::ShootingEnemy; 
if(string == "Shooting_Enemy") return CS_ArcheTypes::Shooting_Enemy; 
if(string == "ShotRate_Item") return CS_ArcheTypes::ShotRate_Item; 
if(string == "Strafing_Enemy") return CS_ArcheTypes::Strafing_Enemy; 
if(string == "White_Bullet") return CS_ArcheTypes::White_Bullet; 
if(string == "White_Particle") return CS_ArcheTypes::White_Particle; 
if(string == "Yellow_Particle") return CS_ArcheTypes::Yellow_Particle; 
return CS_ArcheTypes::INVALID_ArcheType; 
} 
inline std::string ArcheTypeToString(const CS_ArcheTypes type) { 
if(type == CS_ArcheTypes::Barrel_Item) return "Barrel_Item"; 
if(type == CS_ArcheTypes::Black_Bullet) return "Black_Bullet"; 
if(type == CS_ArcheTypes::Black_Particle) return "Black_Particle"; 
if(type == CS_ArcheTypes::Blue_Particle) return "Blue_Particle"; 
if(type == CS_ArcheTypes::Bubble) return "Bubble"; 
if(type == CS_ArcheTypes::Bullet) return "Bullet"; 
if(type == CS_ArcheTypes::ChasingEnemy) return "ChasingEnemy"; 
if(type == CS_ArcheTypes::Chasing_Enemy) return "Chasing_Enemy"; 
if(type == CS_ArcheTypes::DMG_Item) return "DMG_Item"; 
if(type == CS_ArcheTypes::Enemy) return "Enemy"; 
if(type == CS_ArcheTypes::GenEffect) return "GenEffect"; 
if(type == CS_ArcheTypes::HP_Item) return "HP_Item"; 
if(type == CS_ArcheTypes::Map) return "Map"; 
if(type == CS_ArcheTypes::NewObject) return "NewObject"; 
if(type == CS_ArcheTypes::Particle) return "Particle"; 
if(type == CS_ArcheTypes::Player) return "Player"; 
if(type == CS_ArcheTypes::ShootingEnemy) return "ShootingEnemy"; 
if(type == CS_ArcheTypes::Shooting_Enemy) return "Shooting_Enemy"; 
if(type == CS_ArcheTypes::ShotRate_Item) return "ShotRate_Item"; 
if(type == CS_ArcheTypes::Strafing_Enemy) return "Strafing_Enemy"; 
if(type == CS_ArcheTypes::White_Bullet) return "White_Bullet"; 
if(type == CS_ArcheTypes::White_Particle) return "White_Particle"; 
if(type == CS_ArcheTypes::Yellow_Particle) return "Yellow_Particle"; 
if(type == CS_ArcheTypes::Barrel_Item) return "Barrel_Item"; 
if(type == CS_ArcheTypes::Black_Bullet) return "Black_Bullet"; 
if(type == CS_ArcheTypes::Black_Particle) return "Black_Particle"; 
if(type == CS_ArcheTypes::Blue_Particle) return "Blue_Particle"; 
if(type == CS_ArcheTypes::Bubble) return "Bubble"; 
if(type == CS_ArcheTypes::Bullet) return "Bullet"; 
if(type == CS_ArcheTypes::ChasingEnemy) return "ChasingEnemy"; 
if(type == CS_ArcheTypes::Chasing_Enemy) return "Chasing_Enemy"; 
if(type == CS_ArcheTypes::DMG_Item) return "DMG_Item"; 
if(type == CS_ArcheTypes::Enemy) return "Enemy"; 
if(type == CS_ArcheTypes::GenEffect) return "GenEffect"; 
if(type == CS_ArcheTypes::HP_Item) return "HP_Item"; 
if(type == CS_ArcheTypes::Map) return "Map"; 
if(type == CS_ArcheTypes::NewObject) return "NewObject"; 
if(type == CS_ArcheTypes::Particle) return "Particle"; 
if(type == CS_ArcheTypes::Player) return "Player"; 
if(type == CS_ArcheTypes::ShootingEnemy) return "ShootingEnemy"; 
if(type == CS_ArcheTypes::Shooting_Enemy) return "Shooting_Enemy"; 
if(type == CS_ArcheTypes::ShotRate_Item) return "ShotRate_Item"; 
if(type == CS_ArcheTypes::Strafing_Enemy) return "Strafing_Enemy"; 
if(type == CS_ArcheTypes::White_Bullet) return "White_Bullet"; 
if(type == CS_ArcheTypes::White_Particle) return "White_Particle"; 
if(type == CS_ArcheTypes::Yellow_Particle) return "Yellow_Particle"; 
return "INVALID_ArcheType"; 
} 
