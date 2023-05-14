/**
 * @file CS_ComponentBuildFactory.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Component builder management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_ComponentTypes.hpp"
#include <unordered_map>
class CS_Component;
class CS_ComponentBuilder;


class CS_ComponentBuildFactory
{
public:
	~CS_ComponentBuildFactory(void);
	void AddBuilderIntoFactory(CS_ComponentTypes compType, CS_ComponentBuilder* builder);
	void RemoveBuilderFromFactory(CS_ComponentTypes compType);
	CS_Component* BuildComponentFromFactory(CS_ComponentTypes compType);
	void ClearBuildersFromFactory(void);
private:
	typedef std::unordered_map<CS_ComponentTypes, CS_ComponentBuilder*> BuilderMap;
	BuilderMap m_componentBuilderMap;
};