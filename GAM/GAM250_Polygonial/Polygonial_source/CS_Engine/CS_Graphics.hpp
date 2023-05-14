/**
 * @file CS_Graphics.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Graphics control functions.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include "Transform_Component.hpp"
#include "CS_Color.hpp"

class CS_Input;
class Graphics_Component;
class CS_Application;
class CS_StageManager;
class CS_Camera;
enum class DrawSpace:int
{
	Hud,
	World,
	Invalid
};

enum class ShaderTypes : int
{
    SolidColor = 0,        // Used to display a mesh with one solid color
    InterpolatedColor,     // Used to display a mesh with a color at each point
    TextureWithSolidColor, // Used to display a mesh with a texture and multiply the texel by a uniform color
    Count
};
struct VertexPosition2fTexture2f
{
    glm::vec2 position;
    glm::vec2 textureCoordinate;
};
class CS_Graphics
{    
	friend CS_Input;
    friend CS_Application;
	friend CS_StageManager;
	friend CS_Camera;
	friend Graphics_Component;
private:
	static void UpdateTextureCount(int texureHandle);
public:
    static int LoadTexture(std::string& file_path);
	static std::string GetFilePath(int texureHandle);
    static void DeleteTexture(int texureHandle);
    static void UseShader(ShaderTypes type);
    static void BindTextureToDraw(int textureHandle, int slot);
    static void Draw(Transform_Component& transform, const std::vector<VertexPosition2fTexture2f>& to_draw, int DrawType, const CS_Color& color = CS_Color{ 255,255,255 });
	static glm::vec2 GetDisplayAreaSize(void);
private:
	static DrawSpace GetCurrentDrawingSpace();
	static void StartWorld();
	static void EndWorld();
	static void StartHud();
	static void EndHud();
    static void DescribeVertexPosition2fTexture2fToOpenGL();
    static bool init(void);
    static void StartScene(void);
    static void EndScene(void);
    static void Update(void);
    static void Pause(bool drawPaused = false);
    static void Resume(void);
    static void SetDisplayAreaSize(int width, int height);
    static void SetCameraFromStage(const CS_Camera& newCamera);
    static void AddGfxComponent(Graphics_Component* pGfxComp);
    static void RemoveGfxComponent(Graphics_Component* pGfxComp);
	static CS_Camera& GetViewCamera();
	static CS_Camera& GetHUDCamera();
};

