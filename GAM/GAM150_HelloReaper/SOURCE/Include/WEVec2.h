/******************************************************************************/
/*!
\file   WEVec2.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

Prototypes for the WEVec2 Struct

*/
/******************************************************************************/
#ifndef WE_VEC2_H
#define WE_VEC2_H

/*! A 2D vector for a 2D game, use a separate variable for Z-Order. 
Can be used C style with static functions or C++ with methods.*/
struct WEVec2
{
  float x;/*!< The x coordinate of the vector*/
  float y;/*!< The y coordinate of the vector*/


  //C++ additions **********************************************************

  //Operators
  /*Adds two vectors*/
  WEVec2  operator+(const WEVec2& rhs) const;
  /*Subtracts two vectors*/
  WEVec2  operator-(const WEVec2& rhs) const;
  /*Negates a vector*/
  WEVec2 operator-(void) const;
  /*Scales a vector*/
  WEVec2  operator*(float scale)       const;
  /*Adds a vector to this vector*/
  WEVec2& operator+=(const WEVec2& rhs);
  /*Subtracts a vector from this vector*/
  WEVec2& operator-=(const WEVec2& rhs);
  /*Scales this vector*/
  WEVec2& operator*=(float scale);
  /*Negates this vector*/
  WEVec2& operator-(void);
  /*Tests if two vectors are equal*/
  bool    operator==(const WEVec2& rhs) const;
  /*Tests if two vectors are not equal*/
  bool    operator!=(const WEVec2& rhs) const;

  //Member functions **********************************************************
  /*Normalizes this vector*/
  void Normalize(void);
  /*Allows user to quickly set x and y*/
  void  Set(float xx = 0.f, float yy = 0.f);
  /*Performs the dot product on two vectors*/
  float Dot(const WEVec2& rhs) const;
  /*Gets the Z component of the cross product of two vectors*/
  float CrossZ(const WEVec2& rhs) const;
  /*Gets the length of the vector*/
  float Length(void) const;
  /*Gets the squared length of the vector*/
  float LengthSquared(void) const;
  /*Treats the vector as a point and gets the distace to another pointer*/
  float Distance(const WEVec2& point) const;
  /*Gets the squared distance between two points*/
  float DistanceSquared(const WEVec2& point) const;
  /*Checks if this is the zero vector*/
  bool  IsZero(void) const;

  //Non member functions*******************************************************
  /*Allows user to quickly set x and y*/
  static void Set(WEVec2& result, float x, float y);
  /*Negates a vector*/
  static void Negate(WEVec2& result, const WEVec2& toNegate);
  /*Adds two vectors together*/
  static void Add(WEVec2& result, const WEVec2& vec1, const WEVec2& vec2);
  /*Subtracts two vectors*/
  static void Sub(WEVec2& result, const WEVec2& vec1, const WEVec2& vec2);
  /*Scales a vector*/
  static void Scale(WEVec2& result, const WEVec2& toScale, float scale);
  /*Normalizes a vector*/
  static void Normalize(WEVec2& result, const WEVec2& toNormalize);
  /*Gets the projection vector of vec1 onto vec2*/
  static void Project(WEVec2& result, const WEVec2& vec1, const WEVec2& vec2);
  /*Gets the perpendicular projection vector of vec1 onto vec2*/
  static void PerpProject(WEVec2& result, const WEVec2& vec1, const WEVec2& vec2);
  /*Linearly interpolates a vector based on time*/
  static void Lerp(WEVec2& result, const WEVec2& vec1, const WEVec2& vec2,
    float time);
  /*Gets the Z component of the cross product */
  static float CrossZ(const WEVec2& vec1, const WEVec2& vec2);
  /*Calculates the dot product of two vectors*/
  static float Dot(const WEVec2& vec1, const WEVec2& vec2);
  /*Get the Length of a vector*/
  static float Length(const WEVec2& vec);
  /*Gets the squared length of the vector*/
  static float LengthSquared(const WEVec2& vec);
  /*Get the Distance between two points.*/
  static float Distance(const WEVec2& vec1, const WEVec2& vec2);
  /*Gets the squared distance between two points*/
  static float DistanceSquared(const WEVec2& vec1, const WEVec2& vec2);
  /*Tests if two vectors are equal*/
  static bool IsEqual(const WEVec2& vec1, const WEVec2& vec2);
  /*Tests if two vectors are not equal*/
  static bool IsNotEqual(const WEVec2& vec1, const WEVec2& vec2);
  /*Test if this is the zero vector*/
  static bool IsZero(const WEVec2& vec);

  //friend function************************************************************
  /*Allows users to scale a vector*/
  friend WEVec2 operator*(float scale, const WEVec2& rhs);

};




#endif