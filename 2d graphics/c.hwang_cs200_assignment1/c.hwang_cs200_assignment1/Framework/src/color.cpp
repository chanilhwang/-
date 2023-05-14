/******************************************************************************/
/*!
\file   color.cpp
\par    Purpose: Color class implementation
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   12/9/2014
*/
/******************************************************************************/
// Includes
#include "color.hpp"


/******************************************************************************/
/*!
\fn     std::ostream& operator<<(std::ostream& os, const Color& c)
\brief  Overloaded output operator <<
\param  os
        Reference to an ostream object.
\param  c
        Reference to Color type.
\return 
        Object of type ostream reference with value equivalent to the 
        ostream object against which the output operator is applied.
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Color& c)
{
    os << static_cast<int>(c.mColorRGBA[R]);
    os << " " << static_cast<int>(c.mColorRGBA[G]);
    os << " " << static_cast<int>(c.mColorRGBA[B]);
    os << " " << static_cast<int>(c.mColorRGBA[A]) << "\n";
    return os;
}


/******************************************************************************/
/*!
\fn     std::istream& operator>>(std::istream& is, Color& c)
\brief  Overloaded input operator >>
\param  is
        Reference to an istream object.
\param  c
        Reference to Color type.
\return 
        Object of type istream reference with value equivalent to the 
        istream object against which input operator is applied.
*/
/******************************************************************************/
std::istream& operator>>(std::istream& is, Color& c)
{
    char str[100];
    is.getline(str, 100);

    int r, g, b, a;
    sscanf_s(str, " %d %d %d %d", &r, &g, &b, &a);

    c.SetRGBA(  static_cast<unsigned char>(r),
                static_cast<unsigned char>(g),
                static_cast<unsigned char>(b),
                static_cast<unsigned char>(a));
    
    return is;
}

