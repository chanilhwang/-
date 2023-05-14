/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: abstract-element.h
Purpose: make abstract element to inherit to elements
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_a3
Author: c.hwang / Hwang Chan Il
Creation date: 11/03/2018
- End Header ----------------------------------------------------------------
*/
#ifndef ABSTRACT_ELEMENT
#define ABSTRACT_ELEMENT

#include "CRTP.h"

namespace CS225 {

	class AbstractElement : public ObjectCounter<AbstractElement>
	{
	public:
		int GetID() const { return id; }
		void SetID(int new_id) {id = new_id;}
		virtual int Get() const = 0;
		virtual void Set(int new_val) = 0;
		virtual void Print() const = 0;
		virtual ~AbstractElement() {}
	private:
		int id = 1;
	};
}
#endif