/**
 * @file CS_StageBuildFactory.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Building stage using factory.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once

#include <unordered_map>
#include "CS_StageTypes.hpp"

//Forward declarations
class CS_Stage;
class CS_StageBuilder;

/*! Class for Easily Creating Stages at runtime.*/
class CS_StageBuildFactory
{
public:
    ~CS_StageBuildFactory(void);
    void AddBuilderIntoFactory(CS_StageTypes stageType, CS_StageBuilder* stageBuilder);
    void RemoveBuilderFromFactory(CS_StageTypes stageType);
    CS_Stage* BuildStageFromFactory(CS_StageTypes stageType);
    void ClearBuildersFromFactory(void);

private:
    typedef std::unordered_map<CS_StageTypes, CS_StageBuilder*> StageBuilderMap;
    StageBuilderMap m_stageBuilderMap;
};