/**
 * @file TextureManager.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Texture Manager
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_TextureManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#pragma warning(push)
#pragma warning(disable : 4505) // unreferenced local function has been removed
#pragma warning(disable : 4100) // unreferenced formal parameter
#include "stb_image.h"
#pragma warning(pop)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"
#include "GL/glew.h"
#include <iostream>

void CS_Image::ResizeToPixelWidthHeight(int pixel_width, int pixel_height)
{
    pixels.clear();
    width = pixel_width;
    height = pixel_height;
    pixels.resize(pixel_width*pixel_height);
}

bool CS_Image::LoadFromPNG(const std::string & file_path)
{
    int Channels = ChannelsPerColor;
    stbi_uc *data = stbi_load(file_path.c_str(), &width, &height, &Channels, ChannelsPerColor);
    if (data)
    {
        ResizeToPixelWidthHeight(width, height);
        for (int x = 0; x<width*height; ++x)
        {
            int i = x * ChannelsPerColor;
            pixels[x] = { data[i],data[i + 1],data[i + 2],data[i + 3] };
        }
        stbi_image_free(data);
        return true;
    }
    return false;
}

const CS_Color * CS_Image::GetPixelsPointer() const
{
    assert(pixels.empty() == false);
    return &pixels.front();
}


CS_TextureManager::~CS_TextureManager()
{
    Clear();
}

int CS_TextureManager::LoadTexture(const std::string & file_path)
{
    //if there is loaded texture we don't need to make more
    auto found = m_textureMap.find(file_path);
    if (found != m_textureMap.end())
    {
        ++(found->second.count);
        return found->second.texureHandle;
    }

    CS_Image image;
    if (!image.LoadFromPNG(file_path))
        return -1;
    int texureHandle = -1;
    const int width = image.GetWidth();
    const int height = image.GetHeight();
    glGenTextures(1, (GLuint*)&texureHandle);

    glBindTexture(GL_TEXTURE_2D, texureHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    constexpr int base_mipmap_level = 0;
    constexpr int zero_border = 0;

    glTexImage2D(GL_TEXTURE_2D, base_mipmap_level, GL_RGBA8, width, height, zero_border, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPointer());

    if(texureHandle!=-1)
        m_textureMap.insert(std::make_pair(file_path, CS_Texture(file_path, texureHandle)));// little bit faster than [] 
    
    return texureHandle;
}

void CS_TextureManager::UpdateTextureCount(int texureHandle)
{
	for (auto& FilePathAndTexture : m_textureMap)
	{
		if(FilePathAndTexture.second.texureHandle==texureHandle)
		{
			++FilePathAndTexture.second.count;
			break;
		}
	}
}

void CS_TextureManager::DeleteTexture(int texureHandle)
{
	
    if(texureHandle==-1)
        return;
    for (auto& FilePathAndTexture : m_textureMap)
        if(FilePathAndTexture.second.texureHandle==texureHandle)
        {
            --FilePathAndTexture.second.count;
            if(FilePathAndTexture.second.count==0)
            {
                glDeleteTextures(1, (GLuint*)&texureHandle);
                m_textureMap.erase(FilePathAndTexture.first);
            }
			break;
        }
}

std::string CS_TextureManager::GetFilePath(int texureHandle)
{
	assert(texureHandle != -1);
	for (auto& FilePathAndTexture : m_textureMap)
	{
		if (FilePathAndTexture.second.texureHandle == texureHandle)
		{
			return FilePathAndTexture.first;
		}
	}
	return "error";
}

void CS_TextureManager::Clear()
{
    for (auto& FilePathAndTexture : m_textureMap)
        glDeleteTextures(1, (GLuint*)&FilePathAndTexture.second.texureHandle);
    m_textureMap.clear();
}
