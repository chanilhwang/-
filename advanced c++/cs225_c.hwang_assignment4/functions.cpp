/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: functions.cpp
Purpose: to implement functions.h
Language: c++
Platform: windows 10 64bit, visual studio 2017
Project: c.hwang_cs225_assignment4
Author: c.hwang / HWang Chan Il
Creation date: 11/13/2018
- End Header ----------------------------------------------------------------
*/

#include <iostream>
#include "functions.h"

namespace CS225 {
    template <typename T>
    void display( T const *begin, T const *end )
    {
        if( begin != end ) std::cout << *begin++;
        while( begin<end ) {
            std::cout << ", " << *begin;
            ++begin;
        }
        std::cout << std::endl;
    }

    template <typename T>
    void swap( T *left, T *right )
    {
        T temp( *right );
        ( *right ) = ( *left );
        ( *left ) = temp;
    }

    template <typename T>
    T* min_element( T *begin, T *end )
    {
        T* min=begin;
        while( begin!=end ) {
            if( *begin<*min ) min=begin;
            ++begin;
        }
        return min;
    }

    template <typename T>
    T const* min_element( T const *begin, T const *end )
    {
        T const* min=begin;
        while( begin!=end ) {
            if( *begin<*min ) min=begin;
            ++begin;
        }
        return min;
    }

    template <typename T>
    void swap_ranges( T *left, T *left_end, T *right )
    {
        T *temp = left;
        T *temp2 = right;
        while (left != left_end)
        {
            swap(left, right);
            left++;
            right++;
        }
        left = temp;
        right = temp2;
    }

    template < typename T >
    T* remove( T *begin, T *end, T item )
    {
        unsigned int count = 0;
        for(int i = 0; begin + i < end ; i++)
        {
            if (*(begin + i) != item)
            {
                *(begin + count++) = *(begin + i);
            }
        }
        for(int i = 0; begin + count + i < end; i++)
        {
            *(begin + count + i) = item;
        }
        return (begin + count);
    }

    template < typename T >
    int count( T *begin, T *end, T item )
    {
        int count = 0;
        while (begin != end)
        {
            if (*begin == item)
                count++;
            begin++;
        }
        return count;
    }

    template < typename T >
    T * find( T *begin, T *end, T item )
    {
        while(begin != end)
        {
            if (*begin == item)
                return begin;
            begin++;
        }
        return begin;
    }

    template < typename T >
    T const* find( const T *begin, const T *end, const T item )
    {
        while(begin != end)
        {
            if (*begin == item)
                return begin;
            begin++;
        }
        return begin;
    }

    template < typename T1, typename T2 >
    T2 * copy(T1 *left, T1 *left_end, T2 *right)
    {
        while (left < left_end)
        {
            *right = *left;
            left++;
            right++;
        }

        return right;
    }

    template < typename T >
    void fill( T *begin, T *end, T item )
    {
        T *temp = begin;
        while (begin != end)
        {
            *begin = item;
            begin++;
        }
        begin = temp;
    }

    template < typename T >
    void replace( T *begin, T *end, T olditem, T newitem )
    {
        T *temp = begin;
        while(begin != end)
        {
            if(*begin == olditem)
                *begin = newitem;
            begin++;
        }
        begin = temp;
    }

    template <typename T>
    T* max_element( T *begin, T *end )
    {
        T* max=begin;
        while( begin!=end ) 
        {
            if( *begin>*max )
                max=begin;
            ++begin;
        }
        return max;
    }

    template <typename T>
    T const* max_element( T const *begin, T const *end )
    {
        T const* max=begin;
        while( begin!=end ) 
        {
            if( *begin>*max )
                max=begin;
            ++begin;
        }
        return max;
    }

    template <typename T1, typename T2>
    bool equal( T1 *left, T1 *left_end, T2 *right )
    {
        bool result = false;
        while (left != left_end)
        {
            if (*left == *right)
            {
                result = true;
            }
            else
            {
                result = false;
                return result;
            }
            left++;
            right++;
        }
        return result;
    }

    template < typename T >
    T sum( T *begin, T *end )
    {
        T result = 0;
        while (begin < end)
        {
            result += *begin++;
        }
        return result;
    }
}
