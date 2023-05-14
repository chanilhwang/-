/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: array.h
Purpose: header of array
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_a3
Author: c.hwang / Hwang Chan Il
Creation date: 11/03/2018
- End Header ----------------------------------------------------------------
*/
#ifndef ARRAY_H
#define ARRAY_H
#include "abstract-element.h"
#include "element-factory.h"
#include <iostream>

namespace CS225 {

/*
 * Class Array implements a simple array-like data structure.
 * Objects inside the Array have to be derived from AbstractElement.
 */
	class Array {
		public:
			Array(int * array, unsigned int _size, 
					const ElementFactory* _pElementFactory
					);
			Array(const Array& rhs);
			Array& operator=(Array& rhs);

			int Get(unsigned int pos) const;

			void Set(int id, int pos, int value);	

			void Print() const;

			~Array();
		private:
			AbstractElement** data;
			unsigned int size;
			const ElementFactory* pElementFactory;
			unsigned int* ref_counter;
	};
}
#endif
