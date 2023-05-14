/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: array.cpp
Purpose: implementation of array
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_a3
Author: c.hwang / Hwang Chan Il
Creation date: 11/03/2018
- End Header ----------------------------------------------------------------
*/
#include "array.h"

CS225::Array::Array(int * array,
	unsigned int _size,
	const ElementFactory* _pElementFactory
)
	: data(new AbstractElement*[_size]), size(_size),
	pElementFactory(_pElementFactory), ref_counter(new unsigned int(1))
{
	for (unsigned int i=0;i<size;++i) {
		//create Element with id = 1 (that is Element1 )
		data[i] = pElementFactory->MakeElement( 1, array[i] );
	}
}

CS225::Array::Array(const Array& rhs)
	: data(rhs.data), size(rhs.size),
		pElementFactory(rhs.pElementFactory), ref_counter(rhs.ref_counter)
{ 
	*ref_counter += 1;
}

CS225::Array& CS225::Array::operator=(Array& rhs)
{
	if (data != rhs.data)
	{
		if(*ref_counter == 1)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				delete data[i];
			}
			delete [] data;
			delete ref_counter;
		}
		else
		{
			*ref_counter -= 1;
		}
		ref_counter = rhs.ref_counter;
		*ref_counter += 1;
		data = rhs.data;
		size = rhs.size;
	}
	return *this;
}

int CS225::Array::Get(unsigned int pos) const { return data[pos]->Get(); }

void CS225::Array::Set(int id, int pos, int value) { 
	if (*ref_counter > 1)
	{
		*ref_counter -= 1;
		AbstractElement** new_data = new AbstractElement*[size];
		for (unsigned int i = 0; i < size; ++i) {
			new_data[i] = pElementFactory->MakeElement(data[i]->GetID(), data[i]->Get());
		}
		data = new_data;
		ref_counter = new unsigned int(1);
	}
	delete data[pos];
	data[pos] = pElementFactory->MakeElement(id, value);
	data[pos]->SetID(id);
}

void CS225::Array::Print() const {
	for (unsigned int i=0;i<size;++i) data[i]->Print(); 
	std::cout << std::endl;
}

CS225::Array::~Array()
{
	if (*ref_counter == 1)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			delete data[i];
		}
		delete [] data;
		delete ref_counter;
	}
	else
	{
		*ref_counter -= 1;
	}
}