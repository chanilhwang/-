#pragma once

struct Point {
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	bool operator==(const Point& p) const;
	bool operator!=(const Point& p) const;
	Point operator+(const Point& p) const;

	int x, y;
};

constexpr int INITIAL_COST_SIZE = 999;

struct Cell {
	Cell(int _x, int _y, int _v)
		: p(_x, _y), value(_v),
		f_cost(INITIAL_COST_SIZE), g_cost(INITIAL_COST_SIZE), h_cost(INITIAL_COST_SIZE),
		prev_cell(nullptr), IsChecked(false) {}
	void UpdateCell(int new_g_cost, int new_h_cost);

	Point p;
	int value, f_cost, g_cost, h_cost;
	Cell* prev_cell;
	bool IsChecked;
};