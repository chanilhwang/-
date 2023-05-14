/**
 * @file CS_JsonManager.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief JSON file management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <rapidjson/document.h>
#include <glm/vec2.hpp>
#include <vector>
#include <sstream>
#include <rapidjson/prettywriter.h>
#include "CS_ArcheTypes.hpp"
class CS_JsonObject;

class CS_JsonManager
{
public:
	CS_JsonManager();
	CS_JsonManager(const CS_JsonManager& right);
	CS_JsonManager(CS_JsonManager&& right);
    bool OpenFile(const std::string& file_name, bool makeNew=false);
    void GetObj(const std::string& key, CS_JsonObject& obj);
    void ReadData(const std::string& key, int& val);
    void ReadData(const std::string& key, float& val);
    void ReadData(const std::string& key, std::string& val);
	void ReadData(const std::string& key, std::stringstream& val);
	void ReadData(const std::string& key, CS_ArcheTypes& type);
	void ReadArray(const std::string& key, std::vector<CS_JsonObject>& objects);
	void ReadArray(const std::string& key, std::vector<CS_JsonManager>& objects);
	void Write(const std::string& save_file_name);
	void WriteData(const std::string& key, int val);
	void WriteData(const std::string& key, float val);
	void WriteData(const std::string& key, const std::string& val);
	void WriteData(const std::string& key, glm::vec2& val);
	void WriteArrayPush(const std::string& key, const std::string& val);
	void WriteArrayPush(const std::string& key, CS_JsonObject& obj);
	void WriteObejct(const std::string& key, CS_JsonObject& obj);
	void RemoveAll();
private:
    rapidjson::Document doc;
};
class CS_JsonObject
{
    friend CS_JsonManager;
public:
	CS_JsonObject();
	CS_JsonObject(const CS_JsonObject& right);
	CS_JsonObject(CS_JsonObject&& right);
    void ReadData(const std::string& key, int& val);
	void ReadData(const std::string& key, float& val);
	void ReadData(const std::string& key, std::string& val);
	void ReadData(const std::string& key, glm::vec2& val);
	void ReadData(const std::string& key, CS_ArcheTypes& type);
	void WriteData(const std::string& key, int val);
	void WriteData(const std::string& key, float val);
	void WriteData(const std::string& key, const std::string& val);
	void WriteData(const std::string& key, glm::vec2& val);
	void WriteData(const std::string& key, CS_ArcheTypes type);
	void WriteArrayPush(const std::string& key, const std::string& val);
	void WriteArrayPush(const std::string& key, CS_JsonObject& obj);
	void WriteObejct(const std::string& key, CS_JsonObject& obj);
private:
	rapidjson::Document doc;
};
