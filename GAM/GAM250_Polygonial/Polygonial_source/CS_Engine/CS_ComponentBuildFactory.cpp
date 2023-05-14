/**
 * @file CS_ComponentBuildFactory.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Component builder management.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_ComponentBuildFactory.hpp"
#include "CS_ComponentBuilder.hpp"
#include <iostream>

CS_ComponentBuildFactory::~CS_ComponentBuildFactory(void)
{
	ClearBuildersFromFactory();
}

void CS_ComponentBuildFactory::AddBuilderIntoFactory(CS_ComponentTypes compType, CS_ComponentBuilder * builder)
{
	auto found = m_componentBuilderMap.find(compType);
	if (found != m_componentBuilderMap.end())
	{
		std::cout << "error trying to add same compo builder\n";
		return;
	}
	m_componentBuilderMap.insert(std::make_pair(compType, builder));
}

void CS_ComponentBuildFactory::RemoveBuilderFromFactory(CS_ComponentTypes compType)
{
	auto found = m_componentBuilderMap.find(compType);
	if (found == m_componentBuilderMap.end())
	{
		std::cout << "error trying to remove a Builder which is not exist\n";
		return;
	}
	//First delete the builder
	delete found->second;
	found->second = nullptr;
	//then erase the element
	m_componentBuilderMap.erase(found);
}

CS_Component * CS_ComponentBuildFactory::BuildComponentFromFactory(CS_ComponentTypes compType)
{
	auto found = m_componentBuilderMap.find(compType);
	if (found == m_componentBuilderMap.end())
	{
		std::cout << "error trying to use a Builder which is not exist\n";
		return nullptr;
	}
	return found->second->Build();
}

void CS_ComponentBuildFactory::ClearBuildersFromFactory(void)
{
	for (auto& pair : m_componentBuilderMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	//Then clear the hash table
	m_componentBuilderMap.clear();
}
