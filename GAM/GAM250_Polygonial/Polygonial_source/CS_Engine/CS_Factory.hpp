/**
 * @file CS_Factory.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Builder controls.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <unordered_map>
#include <iostream>
template<typename EnumType, typename BuilderType, typename ReturnType>
class CS_Factory
{
public:
	~CS_Factory(void);
	bool AddBuilder(EnumType type, BuilderType* builder);
	void RemoveBuilder(EnumType type);
	ReturnType* Build(EnumType type);
	void ClearBuilders(void);
private:

	typedef std::unordered_map<EnumType, BuilderType*> BuildersMap;
	BuildersMap m_builderMap;
};


template<typename EnumType, typename BuilderType, typename ReturnType>
CS_Factory<EnumType, BuilderType, ReturnType>::~CS_Factory(void)
{
	ClearBuilders();
}

template<typename EnumType, typename BuilderType, typename ReturnType>
bool CS_Factory<EnumType, BuilderType, ReturnType>::AddBuilder(EnumType type, BuilderType* pBuilder)
{
	auto found = m_builderMap.find(type);
	if (found != m_builderMap.end())
	{
		std::cout << "error trying to add same builder\n";
		return;
	}
	m_builderMap.insert(std::make_pair(type, pBuilder));
}

template<typename EnumType, typename BuilderType, typename ReturnType>
void CS_Factory<EnumType, BuilderType, ReturnType>::RemoveBuilder(EnumType type)
{
	auto found = m_builderMap.find(type);
	if (found == m_builderMap.end())
	{
		std::cout << "error trying to remove a Builder which is not exist\n";
		return;
	}
	//First delete the builder
	delete found->second;
	found->second = nullptr;
	//then erase the element
	m_builderMap.erase(found);
}

template<typename EnumType, typename BuilderType, typename ReturnType>
ReturnType* CS_Factory<EnumType, BuilderType, ReturnType>::Build(EnumType type)
{
	auto found = m_builderMap.find(type);
	if (found == m_builderMap.end())
	{
		std::cout << "error trying to use a Builder which is not exist\n";
		return nullptr;
	}
	return found->second->Build();
}

template<typename EnumType, typename BuilderType, typename ReturnType>
void CS_Factory<EnumType, BuilderType, ReturnType>::ClearBuilders(void)
{
	for (auto& pair : m_builderMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	//Then clear the hash table
	m_builderMap.clear();

}