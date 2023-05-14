#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include "HashMap.h"

void test1() {
	std::cout << "============ Test 1 ===============" << std::endl;
	std::string text = "agi sang-eo\n\
ttu lulus ttulu\n\
gwiyeoun\n\
ttu lulus ttulu\n\
badas - sog\n\
ttu lulus ttulu\n\
agi sang - eo!\n\
eomma sang - eo\n\
ttu lulus ttulu\n\
eoyeoppeun\n\
ttu lulus ttulu\n\
badas - sog\n\
ttu lulus ttulu\n\
eomma sang - eo!\n\
appa sang - eo\n\
ttu lulus ttulu\n\
him - i sen\n\
ttu lulus ttulu\n\
badas - sog\n\
ttu lulus ttulu\n\
appa sang - eo!";
	std::cout << text << std::endl;

	HashMap<char, int> map;
	for (auto& c : text) {
		map[c] += 1;
	}
	for (char c = 'a'; c != 'z' + 1; c++) {
		if(map[c])
			std::cout << c << ": " << map[c] << std::endl;
	}
}


void TestSudoku(char **board) {
	bool IsCompleted = true;
	bool Duplicated = false;
	HashMap<int, int> map;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[board[i][j]]) {
				std::cout << "Error in row: ";
				std::cout << i << std::endl;
				Duplicated = true;
			}
			else {
				if (board[i][j] != -1)
					map[board[i][j]] = 1;
				else
					IsCompleted = false;
			}
		}
		map.Clear();
	}

	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			if (map[board[i][j]]) {
				std::cout << "Error in column: ";
				std::cout << j << std::endl;
				Duplicated = true;
			}
			else {
				if (board[i][j] != -1)
					map[board[i][j]] = 1;
				else
					IsCompleted = false;
			}
		}
		map.Clear();
	}

	//check square
	for (int box_i = 0; box_i < 3; box_i++) {
		for (int box_j = 0; box_j < 3; box_j++) {
			for (int i = box_i * 3; i < (box_i + 1) * 3; i++) {
				for (int j = box_j * 3; j < (box_j + 1) * 3; j++) {
					if (map[board[i][j]]) {
						std::cout << "Error in square: ";
						std::cout << "(" << box_i * 3 << ", " << box_j * 3 << ")" << std::endl;
						Duplicated = true;
					}
					else {
						if (board[i][j] != -1)
							map[board[i][j]] = 1;
						else
							IsCompleted = false;
					}
				}
			}
			map.Clear();
		}
	}
	if (!Duplicated) {
		if (IsCompleted)
			std::cout << "Puzzle solved correctly" << std::endl;
		else
			std::cout << "Puzzle doesn't have any errors at the moment" << std::endl;
	}
}

void test2() {
	std::cout << "============ Test 2 ===============" << std::endl;
	char *raw = new char[9 * 9 + 9 * sizeof(char*)];
	char **sudoku = reinterpret_cast<char**>(raw);
	for (int i = 0; i < 9; i++) {
		sudoku[i] = reinterpret_cast<char*>(&raw[i * 9 + 9 * sizeof(char*)]);
	}
	sudoku[0][0] = 3;	sudoku[0][1] = 5;	sudoku[0][2] = 1;	sudoku[0][3] = 9;	sudoku[0][4] = 2;	sudoku[0][5] = 4;	sudoku[0][6] = 7;	sudoku[0][7] = 6;	sudoku[0][8] = 8;
	sudoku[1][0] = 2;	sudoku[1][1] = 8;	sudoku[1][2] = 4;	sudoku[1][3] = 6;	sudoku[1][4] = 1;	sudoku[1][5] = 7;	sudoku[1][6] = 9;	sudoku[1][7] = 3;	sudoku[1][8] = 5;
	sudoku[2][0] = 9;	sudoku[2][1] = 7;	sudoku[2][2] = 6;	sudoku[2][3] = 5;	sudoku[2][4] = 3;	sudoku[2][5] = 8;	sudoku[2][6] = 4;	sudoku[2][7] = 2;	sudoku[2][8] = 1;

	sudoku[3][0] = 7;	sudoku[3][1] = 4;	sudoku[3][2] = 5;	sudoku[3][3] = 3;	sudoku[3][4] = 8;	sudoku[3][5] = 2;	sudoku[3][6] = 1;	sudoku[3][7] = 9;	sudoku[3][8] = 6;
	sudoku[4][0] = 6;	sudoku[4][1] = 2;	sudoku[4][2] = 8;	sudoku[4][3] = 1;	sudoku[4][4] = 9;	sudoku[4][5] = 5;	sudoku[4][6] = 3;	sudoku[4][7] = 7;	sudoku[4][8] = 4;
	sudoku[5][0] = 1;	sudoku[5][1] = 9;	sudoku[5][2] = 3;	sudoku[5][3] = 4;	sudoku[5][4] = 7;	sudoku[5][5] = 6;	sudoku[5][6] = 8;	sudoku[5][7] = 5;	sudoku[5][8] = 2;

	sudoku[6][0] = 4;	sudoku[6][1] = 6;	sudoku[6][2] = 9;	sudoku[6][3] = 8;	sudoku[6][4] = 5;	sudoku[6][5] = 3;	sudoku[6][6] = 2;	sudoku[6][7] = 1;	sudoku[6][8] = 7;
	sudoku[7][0] = 5;	sudoku[7][1] = 1;	sudoku[7][2] = 2;	sudoku[7][3] = 7;	sudoku[7][4] = 4;	sudoku[7][5] = 9;	sudoku[7][6] = 6;	sudoku[7][7] = 8;	sudoku[7][8] = 3;
	sudoku[8][0] = 8;	sudoku[8][1] = 3;	sudoku[8][2] = 7;	sudoku[8][3] = 2;	sudoku[8][4] = 6;	sudoku[8][5] = 1;	sudoku[8][6] = 5;	sudoku[8][7] = 4;	sudoku[8][8] = 9;

	TestSudoku(sudoku);
	delete raw;
}

void test3() {
	std::cout << "============ Test 3 ===============" << std::endl;
	char *raw = new char[9 * 9 + 9 * sizeof(char*)];
	char **sudoku = reinterpret_cast<char**>(raw);
	for (int i = 0; i < 9; i++) {
		sudoku[i] = reinterpret_cast<char*>(&raw[i * 9 + 9 * sizeof(char*)]);
	}
	sudoku[0][0] = -1;	sudoku[0][1] = 5;	sudoku[0][2] = 1;	sudoku[0][3] = 9;	sudoku[0][4] = 2;	sudoku[0][5] = -1;	sudoku[0][6] = 7;	sudoku[0][7] = -1;	sudoku[0][8] = 8;
	sudoku[1][0] = 2;	sudoku[1][1] = 8;	sudoku[1][2] = -1;	sudoku[1][3] = -1;	sudoku[1][4] = 1;	sudoku[1][5] = -1;	sudoku[1][6] = -1;	sudoku[1][7] = -1;	sudoku[1][8] = -1;
	sudoku[2][0] = 9;	sudoku[2][1] = 7;	sudoku[2][2] = -1;	sudoku[2][3] = -1;	sudoku[2][4] = 3;	sudoku[2][5] = 8;	sudoku[2][6] = -1;	sudoku[2][7] = 2;	sudoku[2][8] = 1;

	sudoku[3][0] = -1;	sudoku[3][1] = -1;	sudoku[3][2] = 5;	sudoku[3][3] = -1;	sudoku[3][4] = 8;	sudoku[3][5] = 2;	sudoku[3][6] = 1;	sudoku[3][7] = 9;	sudoku[3][8] = 6;
	sudoku[4][0] = -1;	sudoku[4][1] = -1;	sudoku[4][2] = 8;	sudoku[4][3] = 1;	sudoku[4][4] = 9;	sudoku[4][5] = 5;	sudoku[4][6] = 3;	sudoku[4][7] = -1;	sudoku[4][8] = -1;
	sudoku[5][0] = 1;	sudoku[5][1] = 9;	sudoku[5][2] = -1;	sudoku[5][3] = 4;	sudoku[5][4] = 7;	sudoku[5][5] = -1;	sudoku[5][6] = 8;	sudoku[5][7] = -1;	sudoku[5][8] = -1;

	sudoku[6][0] = 4;	sudoku[6][1] = 6;	sudoku[6][2] = -1;	sudoku[6][3] = 8;	sudoku[6][4] = 5;	sudoku[6][5] = -1;	sudoku[6][6] = -1;	sudoku[6][7] = 1;	sudoku[6][8] = 7;
	sudoku[7][0] = 5;	sudoku[7][1] = 1;	sudoku[7][2] = -1;	sudoku[7][3] = -1;	sudoku[7][4] = 4;	sudoku[7][5] = -1;	sudoku[7][6] = -1;	sudoku[7][7] = 8;	sudoku[7][8] = 3;
	sudoku[8][0] = 8;	sudoku[8][1] = -1;	sudoku[8][2] = 7;	sudoku[8][3] = -1;	sudoku[8][4] = -1;	sudoku[8][5] = 1;	sudoku[8][6] = 5;	sudoku[8][7] = -1;	sudoku[8][8] = -1;

	TestSudoku(sudoku);
	delete raw;
}

void test4() {
	std::cout << "============ Test 4 ===============" << std::endl;
	char *raw = new char[9 * 9 + 9 * sizeof(char*)];
	char **sudoku = reinterpret_cast<char**>(raw);
	for (int i = 0; i < 9; i++) {
		sudoku[i] = reinterpret_cast<char*>(&raw[i * 9 + 9 * sizeof(char*)]);
	}
	sudoku[0][0] = 3;	sudoku[0][1] = 5;	sudoku[0][2] = 1;	sudoku[0][3] = 9;	sudoku[0][4] = 2;	sudoku[0][5] = 4;	sudoku[0][6] = 7;	sudoku[0][7] = 6;	sudoku[0][8] = 8;
	sudoku[1][0] = 2;	sudoku[1][1] = 3;	sudoku[1][2] = 4;	sudoku[1][3] = 6;	sudoku[1][4] = 1;	sudoku[1][5] = 7;	sudoku[1][6] = 9;	sudoku[1][7] = 3;	sudoku[1][8] = 5;
	sudoku[2][0] = 9;	sudoku[2][1] = 7;	sudoku[2][2] = 6;	sudoku[2][3] = 5;	sudoku[2][4] = 3;	sudoku[2][5] = 8;	sudoku[2][6] = 4;	sudoku[2][7] = 2;	sudoku[2][8] = 2;

	sudoku[3][0] = 7;	sudoku[3][1] = 4;	sudoku[3][2] = 5;	sudoku[3][3] = 3;	sudoku[3][4] = 8;	sudoku[3][5] = 2;	sudoku[3][6] = 1;	sudoku[3][7] = 9;	sudoku[3][8] = 6;
	sudoku[4][0] = 6;	sudoku[4][1] = 2;	sudoku[4][2] = 8;	sudoku[4][3] = 2;	sudoku[4][4] = 9;	sudoku[4][5] = 5;	sudoku[4][6] = 3;	sudoku[4][7] = 7;	sudoku[4][8] = 4;
	sudoku[5][0] = 1;	sudoku[5][1] = 9;	sudoku[5][2] = 3;	sudoku[5][3] = 4;	sudoku[5][4] = 7;	sudoku[5][5] = 6;	sudoku[5][6] = 8;	sudoku[5][7] = 5;	sudoku[5][8] = 2;

	sudoku[6][0] = 4;	sudoku[6][1] = 6;	sudoku[6][2] = 1;	sudoku[6][3] = 8;	sudoku[6][4] = 5;	sudoku[6][5] = 3;	sudoku[6][6] = 2;	sudoku[6][7] = 1;	sudoku[6][8] = 7;
	sudoku[7][0] = 5;	sudoku[7][1] = 1;	sudoku[7][2] = 2;	sudoku[7][3] = 7;	sudoku[7][4] = 4;	sudoku[7][5] = 9;	sudoku[7][6] = 1;	sudoku[7][7] = 8;	sudoku[7][8] = 3;
	sudoku[8][0] = 8;	sudoku[8][1] = 3;	sudoku[8][2] = 7;	sudoku[8][3] = 2;	sudoku[8][4] = 6;	sudoku[8][5] = 1;	sudoku[8][6] = 5;	sudoku[8][7] = 4;	sudoku[8][8] = 9;

	TestSudoku(sudoku);
	delete raw;
}

void test5() {
	std::cout << "============ Test 5 ===============" << std::endl;
	srand(static_cast<unsigned int>(time(NULL)));
	HashMap<int, int> map;
	int total = 0;
	int number_of_people = 0;
	for (int i = 0; i < 1000; i++) {
		while (true) {
			int birthday = rand() % 365;
			if (!map[birthday]) {
				map[birthday] += 1;
				number_of_people++;
			}
			else {
				total += number_of_people;
				number_of_people = 0;
				map.Clear();
				break;
			}
		}
	}
	std::cout << "Average number of test it took was :" << total / 1000.0f << std::endl;
}

int main(int argc, char **argv) {
	int test_num = 0;

	if (argc > 1) {
		test_num = std::atoi(argv[1]);
	}

	switch (test_num) {
	case 1:
		test1();
		break;

	case 2:
		test2();
		break;

	case 3:
		test3();
		break;

	case 4:
		test4();
		break;

	case 5:
		test5();
		break;

	default:
		test1();
		test2();
		test3();
		test4();
		test5();
	}

	system("pause");

	return 0;
}