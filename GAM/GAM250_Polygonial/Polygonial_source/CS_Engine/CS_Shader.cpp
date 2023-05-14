/**
 * @file CS_Shader.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Shader management.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Shader.hpp"
#include "GL/glew.h"
#include "CS_Color.hpp"
bool CS_Shader::LoadFromSource(const std::string & vertex_shader_source_code, const std::string & fragment_shader_source_code)
{
    const auto VERTEX_SHADER_ID = CompileShader(vertex_shader_source_code, GL_VERTEX_SHADER);
    const auto FRAGMENT_SHADER_ID = CompileShader(fragment_shader_source_code, GL_FRAGMENT_SHADER);
    ProgramHandle = glCreateProgram();
    glAttachShader(ProgramHandle, VERTEX_SHADER_ID);
    glAttachShader(ProgramHandle, FRAGMENT_SHADER_ID);
    glLinkProgram(ProgramHandle);
    GLint result = GL_TRUE;
    glGetProgramiv(ProgramHandle, GL_LINK_STATUS, &result);
    return result == GL_TRUE;
}

void CS_Shader::SendUniformVariable(const std::string & variable_name, const glm::mat3 & matrix)
{
    int location = GetUniformLocation(variable_name);
    glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0].x);
}

void CS_Shader::SendUniformVariable(const std::string & variable_name, const glm::vec3 & three_floats)
{
    int location = GetUniformLocation(variable_name);
    glUniform3f(location, three_floats.x, three_floats.y, three_floats.z);
}

void CS_Shader::SendUniformVariable(const std::string & variable_name, float number)
{
    int location = GetUniformLocation(variable_name);
    glUniform1f(location, number);
}

void CS_Shader::SendUniformVariable(const std::string& variable_name, int number)
{
    int location = GetUniformLocation(variable_name);
    glUniform1i(location, number);
}

void CS_Shader::SendUniformVariable(const std::string & variable_name, const CS_Color & color)
{
    int location = GetUniformLocation(variable_name);
    auto vec4 = color.ToRGBAfloat();
    glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void CS_Shader::SendUniformVariable(const std::string & variable_name, bool true_or_false)
{
    int location = GetUniformLocation(variable_name);
    if (true_or_false)
        glUniform1i(location, GL_TRUE);
    else
        glUniform1i(location, GL_FALSE);
}


int CS_Shader::GetVertexAttributeLocation(const std::string & vertex_field_name) const
{
    return glGetAttribLocation(ProgramHandle, vertex_field_name.c_str());
}
unsigned CS_Shader::CompileShader(const std::string & source, int type)
{
    const auto ShaderID = glCreateShader(type);
    const char* str = source.c_str();
    glShaderSource(ShaderID, 1, &str, nullptr);
    glCompileShader(ShaderID);
    return ShaderID;
}
void CS_Shader::MapUniformLocations(const std::string & name)
{
    const auto location = glGetUniformLocation(ProgramHandle, name.c_str());
    UniformLocations[name] = location;
}
int CS_Shader::GetUniformLocation(const std::string & name)
{
    return glGetUniformLocation(ProgramHandle, name.c_str());
}
void CS_Shader::Delete()
{
    if (ProgramHandle != 0)
        glDeleteProgram(ProgramHandle);
    ProgramHandle = 0;
}
void CS_Shader::UseShader(const CS_Shader & shader)
{
    glUseProgram(shader.GetShaderHandler());
}
