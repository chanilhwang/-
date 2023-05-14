/**
 * @file CS_StageBuildFactory.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Building stage using factory.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_StageBuildFactory.hpp"
#include "CS_StageBuilder.hpp"
#include <iostream>

CS_StageBuildFactory::~CS_StageBuildFactory()
{
    ClearBuildersFromFactory();
}

void CS_StageBuildFactory::AddBuilderIntoFactory(CS_StageTypes stageType, CS_StageBuilder * stageBuilder)
{
    auto found = m_stageBuilderMap.find(stageType);
    if (found != m_stageBuilderMap.end())
    {
        std::cout << "error trying to add same builder\n";
        return;
    }
    m_stageBuilderMap.insert(std::make_pair(stageType, stageBuilder));
}

void CS_StageBuildFactory::RemoveBuilderFromFactory(CS_StageTypes stageType)
{
    auto found = m_stageBuilderMap.find(stageType);
    if(found==m_stageBuilderMap.end())
    {
        std::cout << "error trying to remove a Builder which is not exist\n";
        return;
    }
    //First delete the builder
    delete found->second;
    found->second = nullptr;
    //then erase the element
    m_stageBuilderMap.erase(found);
}

CS_Stage * CS_StageBuildFactory::BuildStageFromFactory(CS_StageTypes stageType)
{
    auto found = m_stageBuilderMap.find(stageType);
    if(found==m_stageBuilderMap.end())
    {
        std::cout << "error trying to use a Builder which is not exist\n";
        return nullptr;
    }
    return found->second->Build();
}

void CS_StageBuildFactory::ClearBuildersFromFactory(void)
{
    for (auto& pair:m_stageBuilderMap )
    {
        delete pair.second;
        pair.second = nullptr;
    }
    //Then clear the hash table
    m_stageBuilderMap.clear();
}
