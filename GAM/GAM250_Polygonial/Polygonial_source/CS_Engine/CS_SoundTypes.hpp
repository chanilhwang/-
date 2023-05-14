#pragma once 
#include <string> 
 
 
enum class CS_SoundTypes {  
Activate, 
Back, 
BGM, 
Deactivate, 
EnemyHit, 
Enemy_shoot1, 
Explosion1, 
Explosion2, 
Gatling, 
GenSound, 
machinegun_spin, 
MG_cooldown, 
MG_shoot, 
MG_shot, 
MG_shotend, 
PentagonGen, 
Pistol, 
Pistol_charge, 
PlayerHit, 
RectangleGen, 
Select, 
Shotgun, 
Shotgun_reload, 
Warning, 
INVALID_Sound 
}; 
 
 
inline CS_SoundTypes StringToSound(const std::string& string) { 
if(string == "Activate") return CS_SoundTypes::Activate; 
if(string == "Back") return CS_SoundTypes::Back; 
if(string == "BGM") return CS_SoundTypes::BGM; 
if(string == "Deactivate") return CS_SoundTypes::Deactivate; 
if(string == "EnemyHit") return CS_SoundTypes::EnemyHit; 
if(string == "Enemy_shoot1") return CS_SoundTypes::Enemy_shoot1; 
if(string == "Explosion1") return CS_SoundTypes::Explosion1; 
if(string == "Explosion2") return CS_SoundTypes::Explosion2; 
if(string == "Gatling") return CS_SoundTypes::Gatling; 
if(string == "GenSound") return CS_SoundTypes::GenSound; 
if(string == "machinegun_spin") return CS_SoundTypes::machinegun_spin; 
if(string == "MG_cooldown") return CS_SoundTypes::MG_cooldown; 
if(string == "MG_shoot") return CS_SoundTypes::MG_shoot; 
if(string == "MG_shot") return CS_SoundTypes::MG_shot; 
if(string == "MG_shotend") return CS_SoundTypes::MG_shotend; 
if(string == "PentagonGen") return CS_SoundTypes::PentagonGen; 
if(string == "Pistol") return CS_SoundTypes::Pistol; 
if(string == "Pistol_charge") return CS_SoundTypes::Pistol_charge; 
if(string == "PlayerHit") return CS_SoundTypes::PlayerHit; 
if(string == "RectangleGen") return CS_SoundTypes::RectangleGen; 
if(string == "Select") return CS_SoundTypes::Select; 
if(string == "Shotgun") return CS_SoundTypes::Shotgun; 
if(string == "Shotgun_reload") return CS_SoundTypes::Shotgun_reload; 
if(string == "Warning") return CS_SoundTypes::Warning; 
return CS_SoundTypes::INVALID_Sound; 
} 
inline std::string SoundToString(const CS_SoundTypes type) { 
if(type == CS_SoundTypes::Activate) return "Activate"; 
if(type == CS_SoundTypes::Back) return "Back"; 
if(type == CS_SoundTypes::BGM) return "BGM"; 
if(type == CS_SoundTypes::Deactivate) return "Deactivate"; 
if(type == CS_SoundTypes::EnemyHit) return "EnemyHit"; 
if(type == CS_SoundTypes::Enemy_shoot1) return "Enemy_shoot1"; 
if(type == CS_SoundTypes::Explosion1) return "Explosion1"; 
if(type == CS_SoundTypes::Explosion2) return "Explosion2"; 
if(type == CS_SoundTypes::Gatling) return "Gatling"; 
if(type == CS_SoundTypes::GenSound) return "GenSound"; 
if(type == CS_SoundTypes::machinegun_spin) return "machinegun_spin"; 
if(type == CS_SoundTypes::MG_cooldown) return "MG_cooldown"; 
if(type == CS_SoundTypes::MG_shoot) return "MG_shoot"; 
if(type == CS_SoundTypes::MG_shot) return "MG_shot"; 
if(type == CS_SoundTypes::MG_shotend) return "MG_shotend"; 
if(type == CS_SoundTypes::PentagonGen) return "PentagonGen"; 
if(type == CS_SoundTypes::Pistol) return "Pistol"; 
if(type == CS_SoundTypes::Pistol_charge) return "Pistol_charge"; 
if(type == CS_SoundTypes::PlayerHit) return "PlayerHit"; 
if(type == CS_SoundTypes::RectangleGen) return "RectangleGen"; 
if(type == CS_SoundTypes::Select) return "Select"; 
if(type == CS_SoundTypes::Shotgun) return "Shotgun"; 
if(type == CS_SoundTypes::Shotgun_reload) return "Shotgun_reload"; 
if(type == CS_SoundTypes::Warning) return "Warning"; 
return "INVALID_Sound"; 
} 
