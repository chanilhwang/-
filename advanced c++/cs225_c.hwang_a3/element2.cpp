/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: element2.cpp
Purpose: implementation of element2
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_a3
Author: c.hwang / Hwang Chan Il
Creation date: 11/03/2018
- End Header ----------------------------------------------------------------
*/
#include "element2.h"

CS225::Element2::Element2(int _val) : p_val(new int(_val)) { }

CS225::Element2::Element2(const Element2& rhs) 
	: AbstractElement(), p_val(new int(*rhs.p_val)) 
{ }

CS225::Element2& CS225::Element2::operator=(const Element2& rhs) {
  if (this!=&rhs) {
		delete p_val;
		p_val = new int(*rhs.p_val);
	} 
  return *this;
}

int CS225::Element2::Get() const {
	return *p_val;
}

void CS225::Element2::Print() const { 
  std::cout << "(" << *p_val << ") "; 
}

void CS225::Element2::Set(int new_val) { 
	*p_val = new_val; 
}

CS225::Element2::~Element2() {
	delete p_val;
}
