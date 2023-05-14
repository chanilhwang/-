#pragma once
#include <string>
#include "Cell.h"

class Astar {
public:
	Astar(const std::string& input);
private:
	bool MakeBoard(std::ifstream& stream);
	bool FindPath();
	bool IsCellValid(const Cell* cell) const {
		return cell->p.x >= 0 && cell->p.x < width
			&& cell->p.y >= 0 && cell->p.y < height;
	}
	Cell* GetCell(const Point& p) const {
		return &board[p.x][p.y];
	}

	int width;
	int height;
	Cell* start;
	Cell* end;
	Cell** board;
};