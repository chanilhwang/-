/**
 * @file Bomb_Component.hpp
 * @author Sungmin Moon
 * @email elphior0\@gmail.com
 * @brief Bomb controls.
 * @version 0.1
 * @date 
 *
 * @copyright Copyright (c) 2018
 *
 */
#pragma once
#include "CS_Components.hpp"
#include <glm/glm.hpp>

class Bomb_Component :public CS_Component
{
public:
	Bomb_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	virtual ~Bomb_Component();
	virtual void Update(float dt);
	Bomb_Component* Clone() const override;
	void ShowGui() override;
private:
	CS_Object* explosion;
	std::string animation = "Textures/Explosion_Animation.png";
};