#pragma once
#include "CS_Components.hpp"
#include <glm/vec2.hpp>
/**
 * @file Background_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For background
 * @version 0.1
 * @date 2018-12-14
 * 
 * @copyright Copyright (c) 2018
 * 
 */
class Background_Component :public CS_Component
{
public:
	Background_Component();
	~Background_Component() override;
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	void Update(float dt) override;
	Background_Component* Clone() const override;
	void ShowGui() override;
private:
	float MaxAdvence = 0.5f;
	float CurrentAdvence = 0.f;
	glm::vec2 WindowSize{ 1.f,1.f };
};
