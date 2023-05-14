#include <iostream>
#include <list>

constexpr int MAX_WEIGHT = 0xffff;

struct Point {
	Point(int _x, int _y) : x(_x), y(_y) {}
	bool operator==(const Point& p) {
		return x == p.x && y == p.y;
	}
	bool operator!=(const Point& p) {
		return x != p.x || y != p.y;
	}
	Point operator+(const Point& p) {
		return Point(x + p.x, y + p.y);
	}

	int x, y;
};

struct Cell {
	Cell(int _x, int _y) : point(Point(_x, _y)), previous_point(nullptr), weight(MAX_WEIGHT) {}

	Point point;
	Cell* previous_point;
	int weight;
};

bool IsInBoard(const Point& p, int width, int height) {
	return p.x >= 0 && p.x < width
		&& p.y >= 0 && p.y < height;
}

bool IsInList(std::list<Point>& checked_point, Cell** board, int _x, int _y) {
	for (auto& p : checked_point) {
		if (p == board[_x][_y].point) {
			return true;
		}
	}
	return false;
}

Point FindSmallestCell(std::list<Point>& checked_point, Cell** board, int width, int height) {
	Point smallest(0, 0);
	int compared_weight = MAX_WEIGHT;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (board[i][j].weight < compared_weight) {
				if (IsInList(checked_point, board, i, j)) {
					continue;
				}
				smallest = board[i][j].point;
				compared_weight = board[i][j].weight;
			}
		}
	}
	return smallest;
}

void CoutBoard(Cell** board, int width, int height, const Point& s, const Point& e) {
	for (int i = 0; i < width; i++) {
		std::cout << "| ";
		for (int j = 0; j < height; j++) {
			std::cout << board[i][j].weight << " | ";
		}
		std::cout << std::endl;
	}

	std::list<Point> path;
	Cell* curr = &board[e.x][e.y];
	for (int i = 0; i <= board[e.x][e.y].weight; i++) {
		path.push_back(curr->point);
		curr = curr->previous_point;
	}
	std::cout << "Path is: ";
	for (auto& p : path) {
		std::cout << "(" << p.x << ", " << p.y << "), ";
	}
	std::cout << std::endl;
}

int main() {
	//Make board
	const int width = 8,
		height = 8;
	const unsigned int CellSize = sizeof(Cell) / sizeof(char);
	char* pre_pre_board = new char[width * height * CellSize + sizeof(int*) * height];
	int* pre_board = reinterpret_cast<int*>(pre_pre_board);
	//initialize board
	for (int j = 0; j < height; j++) {
		pre_board[j] = reinterpret_cast<int>(&pre_pre_board[sizeof(int*) * height + j * width * CellSize]);
	}
	Cell** board = reinterpret_cast<Cell**>(pre_board);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			new(&board[i][j]) Cell(i, j);
		}
	}

    //Set starting, ending point
	std::list<Point> checked_point;
	Point starting_point(0, 0);
	Point ending_point(7, 7);
	Point checking_points[8] = {
		Point(-2, -1), Point(-2, 1), Point(-1, 2), Point(-1, -2),
		Point(2, -1), Point(2, 1), Point(1, 2), Point(1, -2)
	};

	Point curr = starting_point;
	int curr_weight = 0;
	board[curr.x][curr.y].weight = curr_weight;

	while (curr != ending_point) {
		for (int i = 0; i < 8; i++) {
			Point temp = curr + checking_points[i];
			if (IsInBoard(temp, width, height)) {
				if (board[temp.x][temp.y].weight > curr_weight) {
					board[temp.x][temp.y].weight = curr_weight + 1;
					board[temp.x][temp.y].previous_point = &board[curr.x][curr.y];
				}
			}
		}
		checked_point.push_back(curr);
		curr = FindSmallestCell(checked_point, board, width, height);
		curr_weight = board[curr.x][curr.y].weight;
	}

	CoutBoard(board, width, height, starting_point, ending_point);

	delete reinterpret_cast<char*>(board);
    return 0;
}