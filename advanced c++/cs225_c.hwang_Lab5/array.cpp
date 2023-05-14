/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: array.cpp
Purpose: implementation of array.h
Language: c++
Platform: visual studio 2017 / window 10 64 bit
Project: c.hwang_cs225_lab5
Author: c.hwang / hwang chan il
Creation date: 11/09/2018
- End Header ----------------------------------------------------------------
*/

namespace CS225 {

//Proxy functions
    template<typename DataType>
    ProxyArray<DataType>::ProxyArray(Array<DataType>& array, int pos) : array(array), pos(pos) {}

    template<typename DataType>
    ProxyArray<DataType>& ProxyArray<DataType>::operator=(DataType const& val)
    {
        array.Insert(pos, val);
        return *this;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

//ArrayFunctions
    template<typename DataType>
    Array<DataType>::Array() : size(0), data(nullptr), ref_count(new unsigned int(1)) {}

    template<typename DataType>
    Array<DataType>::Array(const Array& array) : size(array.size), data(array.data), ref_count(array.ref_count)
    {
        *ref_count += 1;
    }

    template<typename DataType>
    Array<DataType>::~Array()
    {
        if(Ref_Exists())
        {
            *ref_count -= 1;
        }
        else
        {
            delete [] data;
            delete ref_count;
        }
    }

    template<typename DataType>
    ProxyArray<DataType> Array<DataType>::operator[](int pos)
    {
        return ProxyArray<DataType>(*this, pos);
    }

    template<typename DataType>
    DataType const& Array<DataType>::operator[](int pos) const
    {
        return data[pos];
    }

    template<typename DataType>
    Array<DataType>& Array<DataType>::operator=(Array<DataType> const& array)
    {
        if(Ref_Exists()) {
            *ref_count -= 1;
        }
        else
        {
            delete [] data;
            
            delete ref_count;
        }

        size = array.size;
        data = array.data;
        ref_count = array.ref_count;
        *ref_count += 1;
        return *this;
    }

    template<typename DataType>
    void Array<DataType>::Insert( int pos, DataType const& val ) {
        if(Ref_Exists())
        {
            int old_size = size;
            if( pos >= size)
            {
                size = 2*pos + 1;
                DataType* new_data = new DataType[size];
                for( int i = 0; i < old_size; ++i) {
                    new_data[i] = data[i];
                }

                *ref_count -= 1;
                data = new_data;
                ref_count = new unsigned int(1);
            }
            else
            {
                DataType* new_data = new DataType[size];
                for( int i = 0; i < old_size; ++i) {
                    new_data[i] = data[i];
                }

                *ref_count -= 1;
                data = new_data;
                ref_count = new unsigned int(1);
            }
        }
        else
        {
            if ( pos >= size ) {
                int old_size = size;
                size = 2*pos +1;
                DataType* new_data = new DataType[ size ];
                for ( int i=0; i<old_size; ++i ) {
                    new_data[i] = data[i];
                }

                delete [] data;
                data = new_data;
            }
        }
        data[ pos ] = val;
    }

    template<typename DataType>
    bool Array<DataType>::Ref_Exists() const
    {
        return (*ref_count > 1);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
