// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: thrashing.cpp
// Purpose:   To practice improving a loops performance by making it behave nicely for CPU Cache & Virtual Memory.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Thrashing Assignment
// Author:    c.hwang
// Creation date: 2018/12/16

#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

constexpr auto KiloByte = 1024;
constexpr auto MegaByte = 1024 * KiloByte;
constexpr auto Rows = 1000;
constexpr auto Columns = 1 * MegaByte;

unsigned char Data[Rows][Columns];

void WriteValiesToDisk(const char *file_path) {
	ofstream outputfile(file_path, ios::out | ios::binary);
	if (!outputfile) {
		cerr << "Failed to open " << file_path << " for writing!\n";
		exit(-1);
	}
	outputfile.write(reinterpret_cast<char *>(Data), sizeof(Data));
}

void Solution() {
	unsigned char value = 0;
	for (unsigned row = 0; row < Rows; ++row) 
	{
		for (unsigned column = 0; column < Columns; ++column) 
		{
			Data[row][column] = value + Rows * column;
		}
		value++;
	}
	WriteValiesToDisk("values.new.bin");
}

void Problem() {
	unsigned char value = 0;
	for (unsigned column = 0; column < Columns; ++column) {
		for (unsigned row = 0; row < Rows; ++row) {
			Data[row][column] = value++;
		}
	}
	WriteValiesToDisk("values.original.bin");
}

int main(void) {
	auto start = std::chrono::high_resolution_clock::now();
	Solution();
	auto end = std::chrono::high_resolution_clock::now();
	auto diff_seconds = chrono::duration_cast<chrono::seconds>(end - start);
	auto diff_milliseconds =
		chrono::duration_cast<chrono::milliseconds>(end - start) - diff_seconds;
	cout << "Solution Timing " << diff_seconds.count() << "s "
		<< diff_milliseconds.count() << "ms\n";



	start = std::chrono::high_resolution_clock::now();
	Problem();
	end = std::chrono::high_resolution_clock::now();
	diff_seconds = chrono::duration_cast<chrono::seconds>(end - start);
	diff_milliseconds =
		chrono::duration_cast<chrono::milliseconds>(end - start) - diff_seconds;
	cout << "Problem Timing " << diff_seconds.count() << "s "
		<< diff_milliseconds.count() << "ms\n";

	return 0;
}