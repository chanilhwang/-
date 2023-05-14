/**
 * @file CS_ComponentBuilder.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Building components.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
class CS_Component;

//! Base Builder class to create Components via a factory
class CS_ComponentBuilder
{
public:
	~CS_ComponentBuilder() {} //empty virtual destructor
							 //! Virtual Build call that must be overloaded by all Derived Builders
	virtual CS_Component* Build(void) = 0;
private:
};

/*! Templated builder derived class so I don't need to create a Builder for each
Component type*/
template <typename T>
class CS_TemplatedComponentBuilder : public CS_ComponentBuilder
{
public:
	virtual CS_Component* Build(void);
private:
};


//! Creates a new Component of type T
template <typename T>
CS_Component* CS_TemplatedComponentBuilder<T>::Build(void)
{
	return new T();
}
