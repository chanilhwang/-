/**
 * @file CS_Builder.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Virtual builder.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
template <typename Return>
class CS_BaseTemplatedBuilder
{
public:
	~CS_BaseTemplatedBuilder() {} //empty virtual destructor
						 //Virtual Build call that must be overloaded by all Derived Builders
	virtual Return* Build(void) = 0;
private:
};

/*! Templated builder derived class so I don't need to create a Builder for each
Component type*/
template <typename Return, typename T>
class CS_TemplatedBuilder : public CS_BaseTemplatedBuilder<Return>
{
public:
	virtual Return* Build(void);
private:
};


template <typename Return, typename T>
Return* CS_TemplatedBuilder<Return, T>::Build(void)
{
	return new T();
}