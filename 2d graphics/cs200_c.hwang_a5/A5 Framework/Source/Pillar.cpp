////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Pillar.cpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////


#include "Pillar.hpp"

// Includes

namespace CS200
{
////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Pillar default constructor
*/
////////////////////////////////////////////////////////////////////////////////
Pillar::Pillar() { }

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Pillar copy constructor
	@param	const Pillar& rhs
			Input object
*/
////////////////////////////////////////////////////////////////////////////////
Pillar::Pillar(const Pillar& rhs)
{
	if (this != &rhs)
	{
		mAABB	= rhs.mAABB;
		mBC		= rhs.mBC;
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Pillar assignment operator
	@param	const Pillar& rhs
			Input object
	@return	Reference to this
*/
////////////////////////////////////////////////////////////////////////////////
Pillar& Pillar::operator=(const Pillar& rhs)
{
    if(this != &rhs)
    {
		mAABB	= rhs.mAABB;
		mBC		= rhs.mBC;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Pillar virtual destructor
	@param	None
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
Pillar::~Pillar()
{
}

}	// namespace CS200

