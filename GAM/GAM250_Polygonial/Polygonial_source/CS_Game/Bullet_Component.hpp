/**
 * @file Bullet_Component.cpp
 * @author Sungmin Moon / ChanIl Hwang
 * @email elphior0\@gmail.com / tktlssla803@gmail.com
 * @brief Bullet controls.
 * @version 0.1
 * @date 2018-12-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Components.hpp"
#include <glm/glm.hpp>
#include "DMG.h"
#include "Color.hpp"

class Bullet_Component :public CS_Component
{
public:
	Bullet_Component();
	~Bullet_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	CS_Component* Clone() const override;
	void ShowGui() override;
	void Init() override;
	void Update(float dt);
	bool IsDurationEnded(float dt);
	void CheckCollideAndInteract(std::string CollidedObject);
	void TerminateThis();
	int Get_Damage() const;
	void Set_Damage(int new_dmg);
	Color Get_Color() const;
	void Set_Color(Color color);
	void Set_Duration(float duration);
	void SetTarget(std::string target);
private:
	Color color;
	DMG dmg;
	float duration;
	std::string TargetObject;
};