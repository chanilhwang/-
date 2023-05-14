#pragma once
#include "CS_Components.hpp"
#include <glm/vec2.hpp>
/**
 * @file Background_NO_Adv_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For background component
 * @version 0.1
 * @date 2018-12-14
 * 
 * @copyright Copyright (c) 2018
 * 
 */
class Background_NO_Adv_Component :public CS_Component
{
public:
	Background_NO_Adv_Component();
	~Background_NO_Adv_Component() override;
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	void Update(float dt) override;
	Background_NO_Adv_Component* Clone() const override;
	void ShowGui() override;
private:
	glm::vec2 WindowSize{ 1.f,1.f };
};