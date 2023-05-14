#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Astar.h"
#include "HeapArray.h"

constexpr int ADJACENT_COST = 10;
constexpr int DIAGONAL_COST = 14;

Astar::Astar(const std::string& input) : board(nullptr) {
	std::ifstream fileopen(input);
	if (fileopen.is_open()) {
		if (!MakeBoard(fileopen)) {
			std::cout << "Failed to make board" << std::endl;
			return;
		}
		if (!FindPath()) {
			std::cout << "Failed to find path" << std::endl;
			return;
		}
	}
	else {
		std::cout << "Failed to open file" << std::endl;
	}
}

bool Astar::MakeBoard(std::ifstream& stream) {
	std::string str;
	//Get width, height from input
	stream >> str;
	width = atoi(str.c_str());
	stream >> str;
	height = atoi(str.c_str());
	if (width <= 0 || height <= 0) {
		return false;
	}

	//Allocate board
	char* pre_pre_board = new char[width * height * sizeof(Cell) + width * sizeof(int*)];
	int** pre_board = reinterpret_cast<int**>(pre_pre_board);
	for (int i = 0; i < width; i++) {
		pre_board[i] = reinterpret_cast<int*>(&pre_pre_board[sizeof(int*) * width + i * height * sizeof(Cell)]);
	}
	board = reinterpret_cast<Cell**>(pre_board);

	//Initialize board
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			stream >> str;
			new(&board[i][j]) Cell(i, j, atoi(str.c_str()));
		}
	}

	//Get Start, End from input
	Point start_point, end_point;
	stream >> str;
	start_point.x = atoi(str.c_str());
	stream >> str;
	start_point.y = atoi(str.c_str());
	stream >> str;
	end_point.x = atoi(str.c_str());
	stream >> str;
	end_point.y = atoi(str.c_str());
	start = GetCell(start_point);
	end = GetCell(end_point);

	return true;
}

int CalcHcost(const Point& p, const Point& end) {
	int result = 0, x_diff, y_diff;
	x_diff = abs(p.x - end.x);
	y_diff = abs(p.y - end.y);
	while (x_diff != 0 || y_diff != 0) {
		if (x_diff != 0 && y_diff != 0) {
			result += DIAGONAL_COST;
			x_diff--;
			y_diff--;
		}
		else if (x_diff != 0) {
			result += ADJACENT_COST;
			x_diff--;
		}
		else {
			result += ADJACENT_COST;
			y_diff--;
		}
	}
	return result;
}

void CoutPath(std::vector<Point> path, int visited_cells) {
	std::cout << "Found,  Path is: ";
	for (int i = static_cast<int>(path.size()) - 1; i >= 0; i--) {
		const Point& p = path[i];
		std::cout << "(" << p.x << ", " << p.y << ")  ";
	}
	std::cout << std::endl;
	std::cout << "Visited " << visited_cells << " Cells" << std::endl;
}

//Im using heaparray which is only usable for cell so need to change it.
bool Astar::FindPath() {
	int visited_cells = 0;
	HeapArray heap;
	Cell* curr = start;

	const Point cells_to_find[8] = {
	Point(0, 1), Point(1, 1), Point(1, 0), Point(1, -1),
	Point(0, -1), Point(-1, -1), Point(-1, 0), Point(-1, 1)
	};
	while (curr != end) {
		//check all adjacent/diagonal cells
		for (int i = 0; i < 8; i++) {
			Cell* adjacent_cell = GetCell(curr->p + cells_to_find[i]);
			if (IsCellValid(adjacent_cell) && !adjacent_cell->IsChecked) {
				int cost = i % 2 == 0 ? ADJACENT_COST : DIAGONAL_COST;
				//Check it is first cell check or not
				if (adjacent_cell->f_cost == INITIAL_COST_SIZE) {
					heap.insert(adjacent_cell);
					adjacent_cell->UpdateCell(cost, CalcHcost(adjacent_cell->p, end->p));
				}
				else {
					int new_g_cost = curr->g_cost + cost;
					if (adjacent_cell->g_cost > curr->g_cost + cost) {
						adjacent_cell->UpdateCell(new_g_cost, adjacent_cell->h_cost);
					}
				}
			}
		}
		//Check I visit all cells
		if (heap.IsEmpty()) {
			return false;
		}

		//Update stats
		curr->IsChecked = true;
		visited_cells++;

		//Update current cell to the root of heap
		Cell* temp = curr;
		curr = heap.remove(0);
		curr->prev_cell = temp;
	}
	//to count end cell
	visited_cells++;

	//Get a final path
	std::vector<Point> path;
	curr = end;
	while (curr != nullptr) {
		path.push_back(curr->p);
		curr = curr->prev_cell;
	}

	//cout path
	CoutPath(path, visited_cells);
	return true;
}