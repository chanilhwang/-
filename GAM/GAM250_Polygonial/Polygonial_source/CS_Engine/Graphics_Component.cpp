/**
 * @file Graphics_Component.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Graphics_Component component
 * @version 0.1
 * @date 2018-12-14
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "CS_Object.hpp"
#include "Transform_Component.hpp"
#include "Graphics_Component.hpp"
#include "CS_Graphics.hpp"
#include <iostream>
#include "CS_JsonManager.hpp"
#include "imgui.h"
#include "CS_TextureManager.hpp"
#include "CS_ResourceLists.hpp"

Graphics_Component::Graphics_Component()
	:CS_Component(CS_ComponentTypes::Graphics_Component), color(CS_Color{255,255,255})
{
	CS_Graphics::AddGfxComponent(this);
}

Graphics_Component & Graphics_Component::operator=(const Graphics_Component & right)
{
    mesh = right.mesh;
	CopyTextureHandle(right.m_texureHandle);// we have ref counter
	to_draw = right.to_draw;
	ishud = right.ishud;
    return *this;
}

void Graphics_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	std::string drawspace;
	jsonObject.ReadData("drawspace", drawspace);
	if (drawspace == "World")
	{
		draw_space = DrawSpace::World;
		ishud = false;
	}
	if (drawspace == "Hud")
	{
		draw_space = DrawSpace::Hud;
		ishud = false;
	}
    std::string texture;
    jsonObject.ReadData("texture", texture);
    SetTextureHandle(texture);
}

void Graphics_Component::WriteToFile(CS_JsonObject& jsonObject)
{
	std::string drawspace;
	if (draw_space == DrawSpace::World)
		drawspace = "World";
	if (draw_space == DrawSpace::Hud)
		drawspace = "Hud";
	jsonObject.WriteData("drawspace", drawspace);
	std::string texture = "Textures/box.png";
	if (m_texureHandle == -1)
		jsonObject.WriteData("texture", texture);
	else
	{
		texture=CS_Graphics::GetFilePath(m_texureHandle);
		jsonObject.WriteData("texture", texture);
	}
}

Graphics_Component::~Graphics_Component()
{
	CS_Graphics::RemoveGfxComponent(this);
}

void Graphics_Component::Draw(const CS_Color& color) const
{
	if(CS_Graphics::GetCurrentDrawingSpace()!=draw_space)
		return;
    CS_Graphics::UseShader(ShaderTypes::TextureWithSolidColor);
    CS_Graphics::BindTextureToDraw(m_texureHandle, 0);
    CS_Graphics::Draw(m_Parent->transform, to_draw,mesh.GetDrawType(), color);
}

void Graphics_Component::Update(float dt)
{
    //we should opt it later only when there is change
    //for later animation and reverse 
	//usage for mesh mesh.ComputeMatrix({ topleft }, w//1.f, h//1.f);

	mesh.Update();
	to_draw.clear();
	auto size = mesh.GetPointCount();
	for (int i = 0; i < size; ++i)
		to_draw.push_back({ mesh.GetPoint(i),mesh.GetTextureCoordinate(i) });
	AnimationUpdate(dt);
}

bool Graphics_Component::SetTextureHandle(std::string file_path)
{
    //if (m_texureHandle != -1)
    //{
    //    //deletebefore one 
    //    CS_Graphics::DeleteTexture(m_texureHandle);
    //}
    m_texureHandle = CS_Graphics::LoadTexture(file_path);
    return m_texureHandle != -1;
}

bool Graphics_Component::SetTextureHandleWithInt(int texture_handle)
{
	m_texureHandle = texture_handle;
	return true;
}

int Graphics_Component::GetTextureHandle() const
{
    return m_texureHandle;
}

void Graphics_Component::SetAnimation(int NumOfXFrames, int NumOfYFrames, float framePerSec)
{
	isAnimation = true;
	SetFramePerSecond(framePerSec);
	m_animationData.index = 0;
	m_animationData.NumOfXFrames = NumOfXFrames;
	m_animationData.NumOfYFrames = NumOfYFrames;
	m_animationData.TotalFrames = NumOfXFrames * NumOfYFrames;
	m_animationData.Frames.clear();
	m_animationData.Frames.reserve(m_animationData.TotalFrames);
	float d_x = 1.f / static_cast<float>(NumOfXFrames);
	float d_y = 1.f / static_cast<float>(NumOfYFrames);
	for (int i=0; i<m_animationData.TotalFrames;++i)
	{
		int x = i % NumOfXFrames;
		int y = i / NumOfXFrames;
		glm::vec2 leftTop = { x*d_x,y*d_y };
		m_animationData.Frames.push_back({ leftTop,d_x,d_y });
	}
}

void Graphics_Component::SetFramePerSecond(float sec)
{
	m_animationData.FramePerSec = sec;
	m_animationData.TimeStack = 0.f;
}

void Graphics_Component::ShowGui()
{
	if (ImGui::TreeNode("Graphics_Component"))
	{
		ImGui::Checkbox("IS HUD?", &ishud);
		if (ishud)
		{
			draw_space = DrawSpace::Hud;
		}
		else
			draw_space = DrawSpace::World;
		static int selected = 0;
		ImGui::ListBox("select Texture", &selected, CS_TextureList, CS_TextureListSize, -1);
		std::string selected_tex = "Textures//";
		selected_tex += CS_TextureList[selected];
		if(ImGui::Button("Apply"))
		{
			SetTextureHandle(selected_tex);
		}
		ImGui::Image((void*)(intptr_t)m_texureHandle, { 100,100 });
		ImGui::TreePop();
	}
}

void Graphics_Component::SetDrawSpace(DrawSpace space)
{
	draw_space = space;
	if (space == DrawSpace::Hud)
		ishud = true;
	else
		ishud = false;
}

void Graphics_Component::AnimationUpdate(float dt)
{
	if(!isAnimation)
		return;
	m_animationData.TimeStack += dt;
	if(m_animationData.TimeStack>=m_animationData.FramePerSec)
	{
		++m_animationData.index;
		m_animationData.TimeStack = 0.f;
	}

	if (m_animationData.index >= m_animationData.TotalFrames)
		m_animationData.index = 0;

	mesh.ComputeMatrix(m_animationData.Frames[m_animationData.index].LeftTopPos,
		m_animationData.Frames[m_animationData.index].Width,
		m_animationData.Frames[m_animationData.index].Height
	);

}

void Graphics_Component::CopyTextureHandle(int texureHandle)
{
	if (m_texureHandle != 0)
	{
		//unregiester, regiester with gfxEngine
		CS_Graphics::DeleteTexture(m_texureHandle);
	}
	CS_Graphics::UpdateTextureCount(texureHandle);
	m_texureHandle = texureHandle;
}
