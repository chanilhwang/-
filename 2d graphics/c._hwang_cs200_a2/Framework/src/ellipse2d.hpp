// Guard
#ifndef ELLIPSE2D_HPP_
#define ELLIPSE2D_HPP_

// Includes
#include "graphics.hpp"
#include "color.hpp"


////////////////////////////////////////////////////////////////////////////////
/*!
    @class    Ellipse
    @brief    Simple ellipse class 
*/
////////////////////////////////////////////////////////////////////////////////
class Ellipse2D
{
public:
    // No need for default constructor
    // No need for copy constructor
    // No need for destructor
    // No need for overloaded assignment operator

    // Auxiliary constructor
    Ellipse2D(int x, int y, int a, int b, const Color& clr)
        : mCenterX(x), mCenterY(y), mMajorRadius(a), mMinorRadius(b), mColor(clr) { }

    int     GetCenterX() const          { return mCenterX; }
    int     GetCenterY() const          { return mCenterY; }
    int     GetMajorRadius() const      { return mMajorRadius; }
    int     GetMinorRadius() const      { return mMinorRadius; }

    void    DrawMidpoint(Graphics &gl) const;

private:
    int     mCenterX, mCenterY;
    int     mMajorRadius, mMinorRadius;
    Color   mColor;
};    // class Ellipse2D


#endif    // ELLIPSE2D_HPP_