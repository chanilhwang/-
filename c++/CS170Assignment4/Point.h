
#ifndef POINT_H
#define POINT_H

#include <iostream> // istream, ostream


namespace CS170
{

  class Point
  {
    public:
        // Constructors (2)


        // Overloaded operators (14 member functions)


        // Overloaded operators (2 friend functions)
        
    private:
      double m_x; //! The x-coordinate of a Point
      double m_y; //! The y-coordinate of a Point

        // Helper functions
      double DegreesToRadians(double degrees) const;
      double RadiansToDegrees(double radians) const;
  };
  
    // Overloaded operators (2 non-member, non-friend functions)
    
} // namespace CS170

#endif
////////////////////////////////////////////////////////////////////////////////
