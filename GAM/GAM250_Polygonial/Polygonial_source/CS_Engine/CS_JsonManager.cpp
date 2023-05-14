/**
 * @file CS_JsonManager.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief JSON file management.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_JsonManager.hpp"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/pointer.h>
#include <fstream>
#include <iostream>

CS_JsonManager::CS_JsonManager()
{
	doc.SetObject();
}

CS_JsonManager::CS_JsonManager(const CS_JsonManager & right)
{
	doc.CopyFrom(right.doc, doc.GetAllocator());
}

CS_JsonManager::CS_JsonManager(CS_JsonManager && right)
{
	doc = std::move(right.doc);
}

bool CS_JsonManager::OpenFile(const std::string& file_name, bool makeNew)
{
	
    std::ifstream ifstream(file_name);
	if(makeNew)
	{
		if (!ifstream.is_open())
		{
			
			std::ofstream ofstream(file_name);
			if (!ofstream.is_open())
				return false;
			ofstream.close();
			std::ifstream ifstreamForCreated(file_name);
			if (!ifstreamForCreated.is_open())
				return false;
			rapidjson::IStreamWrapper i_stream_wrapper(ifstreamForCreated);
			doc.ParseStream(i_stream_wrapper);
			return true;
		}
		else
		{
			
			rapidjson::IStreamWrapper i_stream_wrapper(ifstream);
			doc.ParseStream(i_stream_wrapper);
		}
	}
	else
	{
		if (!ifstream.is_open())
			return false;
		rapidjson::IStreamWrapper i_stream_wrapper(ifstream);
		doc.ParseStream(i_stream_wrapper);
	}
    
    return true;
}

void CS_JsonManager::GetObj(const std::string& key, CS_JsonObject& obj)
{
    if(!doc.HasMember(key.c_str()))
       return;
	
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(doc, tempDoc.GetAllocator());
	rapidjson::Value value= tempDoc[key.c_str()].GetObject();
	obj.doc.CopyFrom(value, obj.doc.GetAllocator());
}

void CS_JsonManager::ReadData(const std::string& key, int & val)
{
    if (!doc.HasMember(key.c_str()))
        return;
    val = doc[key.c_str()].GetInt();
}

void CS_JsonManager::ReadData(const std::string& key, float & val)
{
    if (!doc.HasMember(key.c_str()))
        return;
    val = doc[key.c_str()].GetFloat();
}

void CS_JsonManager::ReadData(const std::string& key, std::string & val)
{
    if (!doc.HasMember(key.c_str()))
        return;
    val = doc[key.c_str()].GetString();
}

void CS_JsonManager::ReadData(const std::string& key, std::stringstream & val)
{
	if (!doc.HasMember(key.c_str()))
		return;
	auto arr = doc[key.c_str()].GetArray();
	const int size = arr.Size();
	for (int i = 0; i < size; ++i)
		val << ' ' << arr[i].GetString();
}

void CS_JsonManager::ReadData(const std::string& key, CS_ArcheTypes& type)
{
	if (!doc.HasMember(key.c_str()))
		return;
	std::string temp;
	ReadData(key, temp);
	type = StringToArcheType(temp);
}

void CS_JsonManager::ReadArray(const std::string & key, std::vector<CS_JsonObject>& objects)
{
	if (!doc.HasMember(key.c_str()))
		return;
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(doc, tempDoc.GetAllocator());
	rapidjson::Value _array = tempDoc[key.c_str()].GetArray();
	const int size = _array.Size();
	objects.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		auto obj = _array[i].GetObject();
		CS_JsonObject j_obj;
		rapidjson::Value _obj = _array[i].GetObject();
		j_obj.doc.CopyFrom(_obj, j_obj.doc.GetAllocator());
		objects.push_back(std::move(j_obj));
	}
	
}

void CS_JsonManager::ReadArray(const std::string& key, std::vector<CS_JsonManager>& objects)
{
	if (!doc.HasMember(key.c_str()))
		return;
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(doc, tempDoc.GetAllocator());
	rapidjson::Value _array = tempDoc[key.c_str()].GetArray();
	const int size = _array.Size();
	objects.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		auto obj = _array[i].GetObject();
		CS_JsonManager j_obj;
		rapidjson::Value _obj = _array[i].GetObject();
		j_obj.doc.CopyFrom(_obj, j_obj.doc.GetAllocator());
		objects.push_back(std::move(j_obj));
	}
}

void CS_JsonManager::Write(const std::string& save_file_name)
{
	std::ofstream ofstream(save_file_name);
	rapidjson::OStreamWrapper o_stream_wrapper(ofstream);
	
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(o_stream_wrapper);
	doc.Accept(writer);
}

void CS_JsonManager::WriteData(const std::string& key, int val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Value _val(val);
	doc.AddMember(_key.Move(),_val.Move(), allocator);
}

void CS_JsonManager::WriteData(const std::string& key, float val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Value _val(val);
	doc.AddMember(_key.Move(), _val.Move(), allocator);
}

void CS_JsonManager::WriteData(const std::string& key, const std::string& val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Value _val(val.c_str(), allocator);
	doc.AddMember(_key.Move(), _val.Move(), allocator);
}

void CS_JsonManager::WriteData(const std::string & key, glm::vec2 & val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	if (!doc.HasMember(key.c_str()))
	{
		rapidjson::Value _key(key.c_str(), allocator);
		rapidjson::Value json_array(rapidjson::kArrayType);
		json_array.SetArray();
		doc.AddMember(_key, json_array, allocator);
	}
	if (!doc[key.c_str()].IsArray())
		return;
	doc[key.c_str()].Clear();
	rapidjson::Value _val_x(val.x);
	rapidjson::Value _val_y(val.y);
	doc[key.c_str()].PushBack(_val_x,allocator);
	doc[key.c_str()].PushBack(_val_y, allocator);
}

void CS_JsonManager::WriteArrayPush(const std::string& key, const std::string& val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	if(!doc.HasMember(key.c_str()))
	{
		rapidjson::Value _key(key.c_str(), allocator);
		rapidjson::Value json_array(rapidjson::kArrayType);
		json_array.SetArray();
		doc.AddMember(_key, json_array, allocator);
	}
	
	
	if(!doc[key.c_str()].IsArray())
		return;
	rapidjson::Value string(rapidjson::kArrayType);
	string.SetString(val.c_str(),allocator);
	doc[key.c_str()].PushBack(string,allocator);
}

void CS_JsonManager::WriteArrayPush(const std::string& key, CS_JsonObject& obj)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	if (!doc.HasMember(key.c_str()))
	{
		rapidjson::Value _key(key.c_str(), allocator);
		rapidjson::Value json_array(rapidjson::kArrayType);
		json_array.SetArray();
		doc.AddMember(_key, json_array, allocator);
	}
	if (!doc[key.c_str()].IsArray())
		return;
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(obj.doc, tempDoc.GetAllocator());
	rapidjson::Value tempobj = tempDoc.GetObject();
	rapidjson::Value _object(rapidjson::kObjectType);
	_object.CopyFrom(tempobj, allocator);
	doc[key.c_str()].PushBack(_object, allocator);
}

void CS_JsonManager::WriteObejct(const std::string& key, CS_JsonObject& obj)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(obj.doc, tempDoc.GetAllocator());
	rapidjson::Value tempobj = tempDoc.GetObject();
	rapidjson::Value _object(rapidjson::kObjectType);
	_object.CopyFrom(tempobj, allocator);

	doc.AddMember(_key.Move(), _object.Move(), allocator);
}

void CS_JsonManager::RemoveAll()
{
	doc.SetObject();
}


CS_JsonObject::CS_JsonObject()
{
	doc.SetObject();
}

CS_JsonObject::CS_JsonObject(const CS_JsonObject& right)
{
	doc.CopyFrom(right.doc, doc.GetAllocator());
}

CS_JsonObject::CS_JsonObject(CS_JsonObject&& right)
{
	doc = std::move(right.doc);
}

void CS_JsonObject::ReadData(const std::string& key, int & val)
{
    if (!doc.HasMember(key.c_str()))
        return;
    val = doc[key.c_str()].GetInt();
}

void CS_JsonObject::ReadData(const std::string& key, float & val)
{
    if(!doc.HasMember(key.c_str()))
        return;
    val = doc[key.c_str()].GetFloat();
}

void CS_JsonObject::ReadData(const std::string& key, std::string & val)
{
    if (!doc.HasMember(key.c_str()))
        return;
    val = doc[key.c_str()].GetString();
}

void CS_JsonObject::ReadData(const std::string& key, glm::vec2& val)
{
    if (!doc.HasMember(key.c_str()))
        return;
    auto arr= doc[key.c_str()].GetArray();
    if(arr.Size()!=2)
        return;
    val.x = arr[0].GetFloat();
    val.y = arr[1].GetFloat();
}

void CS_JsonObject::ReadData(const std::string & key, CS_ArcheTypes & type)
{
	std::string temp;
	ReadData(key, temp);
	type = StringToArcheType(temp);
}

void CS_JsonObject::WriteData(const std::string & key, int val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(),allocator);
	rapidjson::Value _val(val);
	doc.AddMember(_key.Move(), _val.Move(), allocator);
}

void CS_JsonObject::WriteData(const std::string & key, float val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Value _val(val);
	doc.AddMember(_key.Move(), _val.Move(), allocator);
}

void CS_JsonObject::WriteData(const std::string & key, const std::string & val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Value _val(val.c_str(), allocator);
	doc.AddMember(_key.Move(), _val.Move(), allocator);
}

void CS_JsonObject::WriteData(const std::string& key, glm::vec2& val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	if (!doc.HasMember(key.c_str()))
	{
		rapidjson::Value _key(key.c_str(), allocator);
		rapidjson::Value json_array(rapidjson::kArrayType);
		json_array.SetArray();
		doc.AddMember(_key, json_array, allocator);
	}
	if (!doc[key.c_str()].IsArray())
		return;
	doc[key.c_str()].Clear();
	rapidjson::Value _val_x(val.x);
	rapidjson::Value _val_y(val.y);
	doc[key.c_str()].PushBack(_val_x, allocator);
	doc[key.c_str()].PushBack(_val_y, allocator);
}

void CS_JsonObject::WriteData(const std::string & key, CS_ArcheTypes type)
{
	WriteData(key, ArcheTypeToString(type));
}

void CS_JsonObject::WriteArrayPush(const std::string & key, const std::string & val)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	if (!doc.HasMember(key.c_str()))
	{
		rapidjson::Value _key(key.c_str(), allocator);
		rapidjson::Value json_array(rapidjson::kArrayType);
		json_array.SetArray();
		doc.AddMember(_key, json_array, allocator);
	}


	if (!doc[key.c_str()].IsArray())
		return;
	rapidjson::Value string(rapidjson::kArrayType);
	string.SetString(val.c_str(), allocator);
	doc[key.c_str()].PushBack(string, allocator);
}

void CS_JsonObject::WriteArrayPush(const std::string & key, CS_JsonObject & obj)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	if (!doc.HasMember(key.c_str()))
	{
		rapidjson::Value _key(key.c_str(), allocator);
		rapidjson::Value json_array(rapidjson::kArrayType);
		json_array.SetArray();
		doc.AddMember(_key, json_array, allocator);
	}
	if (!doc[key.c_str()].IsArray())
		return;
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(obj.doc, tempDoc.GetAllocator());
	rapidjson::Value tempobj = tempDoc.GetObject();
	rapidjson::Value _object(rapidjson::kObjectType);
	_object.CopyFrom(tempobj, allocator);
	doc[key.c_str()].PushBack(_object, allocator);
}

void CS_JsonObject::WriteObejct(const std::string & key, CS_JsonObject & obj)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value _key(key.c_str(), allocator);
	rapidjson::Document tempDoc;
	tempDoc.CopyFrom(obj.doc, tempDoc.GetAllocator());
	rapidjson::Value tempobj = tempDoc.GetObject();
	rapidjson::Value _object(rapidjson::kObjectType);
	_object.CopyFrom(tempobj, allocator);

	doc.AddMember(_key.Move(), _object.Move(), allocator);
}
