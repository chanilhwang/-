/**
 * @file CS_Graphics.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Graphics control functions.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Graphics.hpp"
#include "CS_Application.hpp"
#include "CS_TextureManager.hpp"
#include "GL/glew.h"
#include "CS_Shader.hpp"
#include "CS_ShaderSource.hpp"
#include "Graphics_Component.hpp"
#include "Transform_Component.hpp"
#include "CS_Camera.hpp"
#include <iostream>
#include "CS_GUI.hpp"

//for test
namespace GraphicsInfo
{
	CS_TextureManager texture_manager;
	typedef std::vector<Graphics_Component*> Graphics_Components;
	Graphics_Components m_gfx_components;

	constexpr int NumberOfVertexTypes = (int)ShaderTypes::Count;
	GLuint handlesToVertexAttributesArray[NumberOfVertexTypes];
	GLuint handlesToVertexBuffers[NumberOfVertexTypes];
	CS_Shader shader;
	glm::vec2 DisplayAreaSize;
	CS_Camera camera;
	CS_Camera camforhud;
	static int lastBoundTexture = -1;
	DrawSpace currentDrawingSpace = DrawSpace::Invalid;
}



void CS_Graphics::UpdateTextureCount(int texureHandle)
{
	GraphicsInfo::texture_manager.UpdateTextureCount(texureHandle);
}

int CS_Graphics::LoadTexture(std::string& file_path)
{
    return GraphicsInfo::texture_manager.LoadTexture(file_path);
}
std::string CS_Graphics::GetFilePath(int texureHandle)
{
	return GraphicsInfo::texture_manager.GetFilePath(texureHandle);
}
void CS_Graphics::DeleteTexture(int texureHandle)
{
	GraphicsInfo::texture_manager.DeleteTexture(texureHandle);
}
void CS_Graphics::UseShader(ShaderTypes type)
{
    if (type == ShaderTypes::TextureWithSolidColor)
        CS_Shader::UseShader(GraphicsInfo::shader);
}
void CS_Graphics::BindTextureToDraw(int textureHandle, int slot)
{
    
    if(textureHandle<0)
        return;
    if(slot<0)
        return;
    if (GraphicsInfo::lastBoundTexture != textureHandle)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, textureHandle);
	    GraphicsInfo::lastBoundTexture = textureHandle;
    }
}

void CS_Graphics::Draw(Transform_Component & transform, const std::vector<VertexPosition2fTexture2f>& to_draw, int DrawType, const CS_Color & color)
{

    if(to_draw.size()==0)
        return;
    constexpr int texture_slot = 0;
    GraphicsInfo::shader.SendUniformVariable("MtoW", transform.GetModelToWorld());
	if (GraphicsInfo::currentDrawingSpace == DrawSpace::World)
		GraphicsInfo::shader.SendUniformVariable("depth", transform.CalculateWorldDepth());
	else
		GraphicsInfo::shader.SendUniformVariable("depth", -0.9f);
    GraphicsInfo::shader.SendUniformVariable("color", color);
    GraphicsInfo::shader.SendUniformVariable("texture_sampler", texture_slot);

    glBindVertexArray(GraphicsInfo::handlesToVertexAttributesArray[(int)ShaderTypes::TextureWithSolidColor]);

    glBindBuffer(GL_ARRAY_BUFFER, GraphicsInfo::handlesToVertexBuffers[(int)ShaderTypes::TextureWithSolidColor]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPosition2fTexture2f)*to_draw.size(), &to_draw[0], GL_DYNAMIC_DRAW);

    glDrawArrays(DrawType, 0, (GLsizei)to_draw.size());
}
glm::vec2 CS_Graphics::GetDisplayAreaSize(void)
{
	return GraphicsInfo::DisplayAreaSize;
}

DrawSpace CS_Graphics::GetCurrentDrawingSpace()
{
	return GraphicsInfo::currentDrawingSpace;
}

void CS_Graphics::StartWorld()
{
	GraphicsInfo::currentDrawingSpace = DrawSpace::World;
}

void CS_Graphics::EndWorld()
{
	GraphicsInfo::currentDrawingSpace = DrawSpace::Invalid;
}

void CS_Graphics::StartHud()
{
	GraphicsInfo::currentDrawingSpace = DrawSpace::Hud;
}

void CS_Graphics::EndHud()
{
	GraphicsInfo::currentDrawingSpace = DrawSpace::Invalid;
}

void CS_Graphics::DescribeVertexPosition2fTexture2fToOpenGL()
{
    glBindVertexArray(GraphicsInfo::handlesToVertexAttributesArray[(int)ShaderTypes::TextureWithSolidColor]);
    glBindBuffer(GL_ARRAY_BUFFER, GraphicsInfo::handlesToVertexBuffers[(int)ShaderTypes::TextureWithSolidColor]);
    int position_attribute_location = GraphicsInfo::shader.GetVertexAttributeLocation("position");
    int texture_coordinate_attribute_location = GraphicsInfo::shader.GetVertexAttributeLocation("texture_position");

    constexpr int two_components_in_vertex_position = 2;
    constexpr int two_components_in_texture_coordinate = 2;
    constexpr GLenum float_element_type = GL_FLOAT;
    constexpr GLboolean not_fixedpoint = GL_FALSE;
    const void* pos_offset_in_struct = reinterpret_cast<const void*>(offsetof(VertexPosition2fTexture2f, position));
    const void* tex_offset_in_struct = reinterpret_cast<const void*>(offsetof(VertexPosition2fTexture2f, textureCoordinate));

    glVertexAttribPointer(position_attribute_location, two_components_in_vertex_position, float_element_type, not_fixedpoint, sizeof(VertexPosition2fTexture2f), pos_offset_in_struct);
    glEnableVertexAttribArray(position_attribute_location);

    glVertexAttribPointer(texture_coordinate_attribute_location, two_components_in_texture_coordinate, float_element_type, not_fixedpoint, sizeof(VertexPosition2fTexture2f), tex_offset_in_struct);
    glEnableVertexAttribArray(texture_coordinate_attribute_location);
}


bool CS_Graphics::init()
{
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GraphicsInfo::shader.LoadFromSource(ShaderSource::vertex_positiontexture, ShaderSource::fragment_uniformcolortexture);
    glGenVertexArrays(3, GraphicsInfo::handlesToVertexAttributesArray);
    glGenBuffers(3, GraphicsInfo::handlesToVertexBuffers);
    DescribeVertexPosition2fTexture2fToOpenGL();
	GraphicsInfo::currentDrawingSpace = DrawSpace::Invalid;
    return true;
}

void CS_Graphics::StartScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CS_Graphics::EndScene(void)
{
    glFinish();
    CS_Application::SwapBuffer();
}
void CS_Graphics::Update()
{
    StartScene();
    //draw; with compoenent
	StartWorld();
    GraphicsInfo::shader.SendUniformVariable("WtoCtoNDC", 
        GraphicsInfo::camera.CameraToNDC()*GraphicsInfo::camera.WorldToCamera());
    for(auto gfx: GraphicsInfo::m_gfx_components)
    {
        gfx->Draw(gfx->GetColor());
    }
	EndWorld();

	StartHud();
	GraphicsInfo::shader.SendUniformVariable("WtoCtoNDC",
		GraphicsInfo::camforhud.CameraToNDC()*GraphicsInfo::camforhud.WorldToCamera());
	for (auto gfx : GraphicsInfo::m_gfx_components)
	{
		gfx->Draw(gfx->GetColor());
	}
	EndHud();

    CS_GUI::Draw();
    EndScene();
}

void CS_Graphics::Pause(bool drawPaused)
{
    
}

void CS_Graphics::Resume(void)
{
}

void CS_Graphics::SetDisplayAreaSize(int width, int height)
{
    glViewport(0, 0, width, height);
	GraphicsInfo::DisplayAreaSize.x = static_cast<float>(width);
	GraphicsInfo::DisplayAreaSize.y = static_cast<float>(height);
}



void CS_Graphics::SetCameraFromStage(const CS_Camera & newCamera)
{
	GraphicsInfo::camera = newCamera;
}

void CS_Graphics::AddGfxComponent(Graphics_Component * pGfxComp)
{
	GraphicsInfo::m_gfx_components.push_back(pGfxComp);
}

void CS_Graphics::RemoveGfxComponent(Graphics_Component * pGfxComp)
{
	for(auto i=0; i<GraphicsInfo::m_gfx_components.size();++i)
	{
		if (GraphicsInfo::m_gfx_components[i] == pGfxComp)
		{
			GraphicsInfo::m_gfx_components[i] = GraphicsInfo::m_gfx_components[GraphicsInfo::m_gfx_components.size() - 1];
			GraphicsInfo::m_gfx_components.pop_back();
		}
	}
}

CS_Camera& CS_Graphics::GetViewCamera()
{
	return GraphicsInfo::camera;
}

CS_Camera & CS_Graphics::GetHUDCamera()
{
	return GraphicsInfo::camforhud;
}
