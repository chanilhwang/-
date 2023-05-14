
#ifndef POINT_H
#define POINT_H

#include <iostream> // istream, ostream


namespace CS170
{

  class Point
  {
    public:
        // Constructors (2)
		Point();
		Point(double xaxis, double yaxis);

        // Overloaded operators (14 member functions)
		Point operator%(double _d) const;
		double operator-(const Point& _point) const;
		Point operator^(const Point& _point) const;
		Point& operator+=(const Point& _point);
		Point& operator+=(double _d);
		Point operator-(double _d) const;
		Point& operator++();
		Point operator++(int);
		Point& operator--();
		Point operator--(int);
		Point operator-() const;
		Point operator+(double _d) const;
		Point operator+(const Point& _point) const;
		Point operator*(double _d) const;

        // Overloaded operators (2 friend functions)
		friend std::ostream& operator<<(std::ostream &out, const Point& _point);
		friend std::istream& operator >> (std::istream &in, Point& _point);
        
    private:
      double m_x; //! The x-coordinate of a Point
      double m_y; //! The y-coordinate of a Point

        // Helper functions
      double DegreesToRadians(double degrees) const;
      double RadiansToDegrees(double radians) const;
  };
  
    // Overloaded operators (2 non-member, non-friend functions)
	  Point operator*( double _d,  Point& _point);
	  Point operator+( double _d,  Point& _point);
} // namespace CS170

#endif
////////////////////////////////////////////////////////////////////////////////
