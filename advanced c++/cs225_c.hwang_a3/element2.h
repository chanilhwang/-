/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: element2.h
Purpose: header of element2
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_a3
Author: c.hwang / Hwang Chan Il
Creation date: 11/03/2018
- End Header ----------------------------------------------------------------
*/
#ifndef ELEMENT2_H
#define ELEMENT2_H
#include <iostream>
#include "abstract-element.h"

/*
 * A concrete class implementing AbstractElement interface
 * Uses dynamically allocated data.
 */
namespace CS225 {

  class Element2 : public AbstractElement {
    public:
      Element2(int _val);
      Element2(const Element2& rhs);
      Element2& operator=(const Element2& rhs);
      virtual int Get() const;
      virtual void Set(int new_val);
      virtual void Print() const;
      virtual ~Element2();	
    private:
      int * p_val;
  };

}
#endif
