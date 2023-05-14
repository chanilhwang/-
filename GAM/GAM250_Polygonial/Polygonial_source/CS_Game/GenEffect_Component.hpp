/**
 * @file GenEffect_Component.hpp
 * @author Sungmin Moon
 * @email elphior0@gmail.com
 * @brief Enemy generating effect logic
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */

#pragma once
#include "CS_Components.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_SoundManager.hpp"
#include "CS_Object.hpp"
#include "CS_Graphics.hpp"
#include "Enemy_Component.hpp"

class GenEffect_Component :public CS_Component
{
public:
	GenEffect_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	virtual ~GenEffect_Component();
	virtual void Update(float dt);
	GenEffect_Component* Clone() const override;
	void ShowGui() override;
	void SetupTexture(Shape shape, Color color);
	void GenEnemy(Shape shape, Color color);
private:
	Shape current_shape;
	Color current_color;
	float life = 0.3f;
	float timer;
	glm::vec2 size;
};