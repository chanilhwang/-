/**
 * @file TextureManager.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Texture Manager
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <string>
#include <vector>
#include "CS_Color.hpp"
#include <unordered_map>


class CS_Image
{
public:
    void ResizeToPixelWidthHeight(int pixel_width, int pixel_height);
    bool LoadFromPNG(const std::string & file_path);
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    const CS_Color* GetPixelsPointer() const;
private:
    int width = 0;
    int height = 0;
    std::vector<CS_Color> pixels{};
    static const int ChannelsPerColor = sizeof(CS_Color::RGBA32);
};
class CS_TextureManager
{
public:
    ~CS_TextureManager();
    //if it fail it will return neg number -1
    //this will return texureHandle of texture
    int LoadTexture(const std::string & file_path);
	void UpdateTextureCount(int texureHandle);
    void DeleteTexture(int texureHandle);
	std::string GetFilePath(int texureHandle);
    void Clear();
private:
    struct CS_Texture
    {
        CS_Texture(const std::string& file_path, int texureHandle)
            :file_path(file_path),texureHandle(texureHandle),count(1){}
        std::string file_path; //  the loaded texture path
        int texureHandle;     // handle for this texture
        int count;            //The number of times this texture has been loaded.
    };
    typedef std::unordered_map<std::string, CS_Texture> CS_TextureMap;
    CS_TextureMap m_textureMap;
};

