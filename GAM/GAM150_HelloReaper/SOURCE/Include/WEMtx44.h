/******************************************************************************/
/*!
\file   WEMtx44.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

Prototypes for the WEMtx44 Struct

*/
/******************************************************************************/
#ifndef WE_MTX44_H
#define WE_MTX44_H

#include "WEVec2.h"

/*! The number of rows in the matrix*/
const int WE_ROWS = 4;
/*! The number of columns in the matrix*/
const int WE_COLS = 4;

/*! A 4D Matrix for a 2D game, z is used for Z-order.  This matrix uses row
vectors.  This means the members of a vector are placed in each row of the
matrix.
\verbatim
|Xx Xy Xz 0 |
|Yx Yy Yz 0 |
|Zx Zy Zz 0 |
|Tx Ty Tz 1 |
\endverbatim
*/
struct WEMtx44
{
  float m[WE_ROWS][WE_COLS];/*!< Array of 16 floats to represent a 4x4 matrix*/

  //C++ Additions

  //Member functions
  //Sets all of the matrix values to 0
  void MakeZero(void);
  //Sets the matrix to the Identity Matrix
  void MakeIdentity(void);
  //Sets this matrix to a tranlsation matrix with the given values
  void MakeTranslate(float x, float y, float zOrder);
  //Sets this matrix to a tranlsation matrix with the given values
  void MakeTranslate(const WEVec2& trans, float zOrder);
  //Sets the matrix to a scale matrix with the given values
  void MakeScale(float x, float y);
  //Sets this matrix to a scale matrix with the given values
  void MakeScale(const WEVec2& scale);
  //Sets this matrix to a Rotation matrix around the Z axix
  void MakeRotateZ(float radians);
  //Sets this Matrix to scale rotate and translation matrix with the give values
  void MakeTransform(float scaleX, float scaleY, float radians,
    float transX, float transY, float zOrder);
  //Sets this Matrix to scale rotate and translation matrix with the give values
  void MakeTransform(const WEVec2& scale, float radians,
    const WEVec2& trans, float zOrder);

  //Operators
  //Lets the user mulitply two matricies together
  WEMtx44 operator*(const WEMtx44& rhs) const;
  //Lets the user mulitply two matricies together
  WEMtx44& operator*=(const WEMtx44& rhs);
  //Tests if two matricies are the same
  bool operator==(const WEMtx44& rhs) const;
  //Tests if two matrices are not the same
  bool operator!=(const WEMtx44& rhs) const;


  //Non member functions
  //Makes all matrix values 0
  static void MakeZero(WEMtx44& result);
  //Sets the given matrix to the Identity matrix
  static void MakeIdentity(WEMtx44& result);
  //Multiplies two matricies together
  static void Multiply(WEMtx44& result, const WEMtx44& first, const WEMtx44& second);

  //Sets the given matrix to a translate matrix
  static void MakeTranslate(WEMtx44& result,
    float transX, float transY, float zOrder);
  //Sets the given matrix to a scale matrix
  static void MakeScale(WEMtx44& result, float scaleX, float scaleY);
  //Sets the given matrix to a Rotation around the Z axis matrix
  static void MakeRotateZ(WEMtx44& result, float radians);

  //Sets the given matrix to a scale rotate and translate matrix
  static void MakeTransform(WEMtx44& result, float scaleX, float scaleY,
    float radians,
    float transX, float transY,
    float zOrder);
  //Tests if two matricies are the same
  static bool IsEqual(const WEMtx44& mtx1, const WEMtx44& mtx2);


};




#endif