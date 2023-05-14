#include "Cell.h"

bool Point::operator==(const Point& p) const {
	return x == p.x && y == p.y;
}
bool Point::operator!=(const Point& p) const {
	return x != p.x || y != p.y;
}
Point Point::operator+(const Point& p) const {
	return Point(x + p.x, y + p.y);
}

void Cell::UpdateCell(int new_g_cost, int new_h_cost) {
	g_cost = new_g_cost;
	h_cost = new_h_cost;
	f_cost = g_cost + h_cost;
}