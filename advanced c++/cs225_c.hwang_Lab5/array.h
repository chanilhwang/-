/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: array.h
Purpose: template version lazy copy
Language: c++
Platform: visual studio 2017 / window 10 64 bit
Project: c.hwang_cs225_lab5
Author: c.hwang / hwang chan il
Creation date: 11/09/2018
- End Header ----------------------------------------------------------------
*/

#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>

namespace CS225 {

template<typename DataType>
class Array;
template<typename DataType>
class ProxyArray;

template<typename T>
std::ostream& operator<<( std::ostream &out, Array<T> const& array ) {
    for ( int i=0; i<array.size; ++i ) {
        out << array.data[i] << " ";
    }
    return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, ProxyArray<T> const& proxy)
{
    out << (const_cast<Array<T> const &>(proxy.array))[proxy.pos];
    return out;
}

template<typename DataType>
class ProxyArray
{
public:
    ProxyArray(Array<DataType>& array, int pos);

    ProxyArray& operator=(DataType const& val);

    friend std::ostream& operator<< <DataType>(std::ostream& out, ProxyArray<DataType> const& proxy);

private:
    Array<DataType>& array;
    int pos;
};

template<typename DataType>
class Array
{
public:
    Array();
    Array(const Array& array);
    ~Array();

    ProxyArray<DataType> operator[](int pos);
    DataType const& operator[](int pos) const;

    Array& operator=(const Array& array);

    void Insert(int pos, DataType const& val);

    bool Ref_Exists() const;

    friend std::ostream& operator<< <DataType>(std::ostream &out, Array<DataType> const& array);

private:
    int size;
    DataType* data;
    unsigned int* ref_count;
};
}

#include "array.cpp"
#endif