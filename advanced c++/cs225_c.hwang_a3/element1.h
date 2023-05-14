/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: element1.h
Purpose: header of element1
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_a3
Author: c.hwang / Hwang Chan Il
Creation date: 11/03/2018
- End Header ----------------------------------------------------------------
*/
#ifndef ELEMENT1_H
#define ELEMENT1_H
#include <iostream>
#include "abstract-element.h"
/*
 * A concrete class implementing AbstractElement interface
 * Trivial.
 */
namespace CS225 {

  class Element1 : public AbstractElement {
    public:
      Element1(int _val);
      virtual int Get() const;
      virtual void Set(int new_val);
      virtual void Print() const;
    private:
      int val;
  };
}
#endif
