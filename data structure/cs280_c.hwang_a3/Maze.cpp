/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: maze.cpp
Purpose: to make pathfinding class
Project: assignment 3
Author: c.hwang
Created: 2019/04/17
------------------------------------------------------------------------*/
#include <iostream>
#include <cstring>
#include "Maze.h"

Maze::Maze(int width, int height, Point start, Point end) : width(width), height(height), start(start), end(end) {
	char *rawData = new char[width * height * sizeof(**grid) + height * sizeof(*grid)];

	grid = reinterpret_cast<int **>(rawData);
	for(int i = 0; i < height; i++) {
		grid[i] = reinterpret_cast<int *>(&rawData[i * width * sizeof(**grid) + height * sizeof(*grid)]);
	}
	std::memset(&grid[0][0], 0, sizeof(**grid) * width * height);
}

Maze::~Maze( ) {
	delete[] grid;
}

void Maze::PrintGrid( ) {
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			std::cout.width(4);
			std::cout << grid[i][j];
		}
		std::cout << std::endl;
	}
}

bool Maze::Solve( ) {
	return Solve(start, 1);
}

bool Maze::IsValid(Point point)
{
	const int& target = grid[point.y][point.x];
	if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height) return false;
	if (target != 0) return false;
	return true;
}

bool Maze::Solve(Point position, int step) {
	grid[position.y][position.x] = step;
	if (position.x == end.x &&
		position.y == end.y) {
		return true;
	}
	else {
		if (IsValid(Point{ position.x + 1, position.y })) {
			if (Solve(Point{ position.x + 1, position.y }, step + 1))
				return true;
		}
		if (IsValid(Point{ position.x - 1, position.y })) {
			if (Solve(Point{ position.x - 1, position.y }, step + 1))
				return true;
		}
		if (IsValid(Point{ position.x, position.y + 1 })) {
			if (Solve(Point{ position.x, position.y + 1 }, step + 1))
				return true;
		}
		if (IsValid(Point{ position.x, position.y - 1 })) {
			if (Solve(Point{ position.x, position.y - 1 }, step + 1))
				return true;
		}
		return false;
	}
}