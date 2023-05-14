/**
 * @file CS_Shader.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Shader management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
struct CS_Color;
class CS_Shader
{
public:
    CS_Shader() = default;
    ~CS_Shader() = default;

    bool LoadFromSource(const std::string& vertex_shader_source_code,
        const std::string& fragment_shader_source_code);

    void SendUniformVariable(const std::string& variable_name, const glm::mat3& matrix);
    void SendUniformVariable(const std::string& variable_name, const glm::vec3& three_floats);
    void SendUniformVariable(const std::string& variable_name, float number);
    void SendUniformVariable(const std::string& variable_name, int number);
    void SendUniformVariable(const std::string& variable_name, const CS_Color& color);
    void SendUniformVariable(const std::string& variable_name, bool true_or_false);


    int GetVertexAttributeLocation(const std::string& vertex_field_name) const;

    unsigned GetShaderHandler() const { return ProgramHandle; }
    int GetUniformLocation(const std::string& name);
    void Delete();

public:
    static void UseShader(const CS_Shader& shader);


public:
    CS_Shader(const CS_Shader& other) = delete;
    CS_Shader(CS_Shader&& other) noexcept = delete;
    CS_Shader& operator=(const CS_Shader& other) = delete;
    CS_Shader& operator=(CS_Shader&& other) noexcept = delete;

private:
    unsigned CompileShader(const std::string& source, int type);
    void MapUniformLocations(const std::string& name);

private:
    unsigned ProgramHandle = 0;
    std::map<std::string, int> UniformLocations{};
};
