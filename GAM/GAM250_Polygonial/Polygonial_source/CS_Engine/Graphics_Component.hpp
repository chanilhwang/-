/**
 * @file Graphics_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Graphics_Component component
 * @version 0.1
 * @date 2018-12-14
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include "CS_Components.hpp"
#include "CS_Graphics.hpp"
#include <string>
#include "CS_Mesh.hpp"
class Transform_Component;
class CS_Object;
struct AnimationFrameData
{
	glm::vec2 LeftTopPos{ 0.f,0.f };
	float Width = 1.f;
	float Height = 1.f;
};
struct AnimationData
{
	int index = 0;
	int NumOfXFrames = 1;
	int NumOfYFrames = 1;
	int TotalFrames = 1;
	float FramePerSec = 1.f;
	float TimeStack = 0.f;
	std::vector<AnimationFrameData> Frames{};
};

class Graphics_Component : public CS_Component
{
	friend CS_Object;
public:
    Graphics_Component();
    Graphics_Component& operator=(const Graphics_Component& right);
    void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
    ~Graphics_Component() override;
    void Draw(const CS_Color& color = CS_Color{ 255,255,255 }) const;
    void Update(float dt) override;
    Graphics_Component* Clone() const override { return nullptr; }//this is included in object so we shouldn't clone
    bool SetTextureHandle(std::string file_path);
	bool SetTextureHandleWithInt(int texture_handle);
    int GetTextureHandle() const;
    void SetAnimation(int NumOfXFrames, int NumOfYFrames,float framePerSec);
	void SetFramePerSecond(float sec);
	void ShowGui() override;
	void SetDrawSpace(DrawSpace space);
	CS_Color GetColor() const {
		return color;
	}
	void SetColor(const CS_Color& _color) {
		color = _color;
	}
	CS_Mesh mesh;
	mutable DrawSpace draw_space = DrawSpace::World;
	mutable bool ishud = false;
	mutable bool isAnimation = false;
private:
	void AnimationUpdate(float dt);
	void CopyTextureHandle(int texureHandle);
	int m_texureHandle = -1;  //Texture id loaded from graphics.
	std::vector<VertexPosition2fTexture2f> to_draw;
	AnimationData m_animationData;
	CS_Color color;
};

