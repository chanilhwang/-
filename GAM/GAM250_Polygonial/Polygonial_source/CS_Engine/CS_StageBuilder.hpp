/**
 * @file CS_StageBuilder.hpp
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
class CS_Stage;

//! Base Builder class to create Game Stages via a StageFactory
class CS_StageBuilder
{
public:
    virtual ~CS_StageBuilder() {} //empty virtual destructor
                                 //! Virtual Build call that must be overloaded by all Derived Builders
    virtual CS_Stage* Build(void) = 0;
};

/*! Templated builder derived class so I don't need to create a Builder for each
Stage type*/
template <typename T>
class CS_TemplatedStageBuilder : public CS_StageBuilder
{
public:
    virtual CS_Stage* Build(void);
};


//! Creates a new Stage of type T
template <typename T>
CS_Stage* CS_TemplatedStageBuilder<T>::Build(void)
{
    return new T();
}