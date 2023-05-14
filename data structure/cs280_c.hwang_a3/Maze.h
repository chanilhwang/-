/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: maze.h
Purpose: to make pathfinding class
Project: assignment 3
Author: c.hwang
Created: 2019/04/17
------------------------------------------------------------------------*/
#pragma once

class Maze {
public:
	struct Point {
		int x, y;
	};

	Maze(int width, int height, Point start, Point end);
	~Maze( );
	void SetWall(Point point) { grid[point.y][point.x] = WALL; }
	void PrintGrid( );
	bool Solve( );
	bool IsValid(Point point);

private:	
	bool Solve(Point position, int step);

	static const int WALL = -1;
	int width;
	int height;
	const Point start;
	const Point end;
	int **grid;
};