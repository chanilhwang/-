#pragma once
#include <string>
#include "../Dependencies/Math/glm/glm.hpp"
/*! This struct holds game data that is shared between stages. This struct must
have at least one data member in it.*/
struct CS_GameData
{
    int level=0;
    int maxLevels=0;
    std::string menuFile="";
	std::string PlayerWeapon = "";
	glm::vec2 playerpos;
};