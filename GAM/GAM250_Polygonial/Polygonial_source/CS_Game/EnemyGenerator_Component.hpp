/**
 * @file EnemyGenerator_Component.hpp
 * @author Sungmin Moon
 * @email elphior0@gmail.com
 * @brief Enemy generator logic
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
#include "Enemy_Component.hpp"
#include "CS_Graphics.hpp"

struct Wave_Info
{
	bool randomize;
	int num_enemies;
	float gentimer;
	float time_until_next_wave;
	std::vector<Color> color_type;
	std::vector<Shape> shape_type;
};
class EnemyGenerator_Component :public CS_Component
{
public:
	EnemyGenerator_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	virtual ~EnemyGenerator_Component();
	virtual void Update(float dt);
	EnemyGenerator_Component* Clone() const override;
	void ShowGui() override;
	int RandomDice(int max);
	glm::vec2 RandomizePosition();
	void RandomlyGenerateEnemy(float dt);
	void GenerateSetEnemy(Shape given_shape, Color given_color, float dt);
private:
	CS_Camera* cam;
	CS_Object* newEnemy;
	int enemy_counter;
	float timer;
	Wave_Info* current_wave;
	Shape shape;
	Color color;
	std::vector<glm::vec2> position;
	std::vector<Wave_Info> wave_info;
	int wave_num;
	int wave_counter;
	float reposition = 55.f;

	Wave_Info infinite;
	bool InfiniteMode;

	Wave_Info game_end;
	bool GameFinished;
};