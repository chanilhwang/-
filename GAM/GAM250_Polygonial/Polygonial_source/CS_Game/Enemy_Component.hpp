/**
 * @file Enemy_Component.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Enemy Component
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Components.hpp"
#include "CS_SoundManager.hpp"
#include <glm/glm.hpp>
#include "SphereCollider_Component.hpp"
#include "HP.h"
#include "DMG.h"
#include "Color.hpp"

enum Shape
{
	Null_Shape,
	Triangle,
	Rectangle,
	Pentagon,


	Shape_Count
};

class Enemy_Component :public CS_Component
{
public:
	Enemy_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	virtual ~Enemy_Component();
	virtual void Update(float dt);
	Enemy_Component* Clone() const override;
	void ShowGui() override;

	Color GetColor() const;
	int GetDamage() const;

	void SetShape(Shape newShape);
	void SetColor(Color newColor);
	void CheckCollideAndGetDamaged();
	void SetupTexture();

	bool IsCollidedPlayer = false;

private:
	void Rotate(float dt);
	void Die(float dt);
	void StartUpMotion(float dt);
	void MakeMovementAir(float dt);
	void GetDamagedMotion(float dt);
	void CollidedPlayerMotion(float dt);
	Shape shape;
	Color color;
	HP hp;
	DMG dmg;
	bool IsGetDamaged = false;
	bool StartUp = false;
	float StartUpMotionDuration;
	float timer = 0;
};