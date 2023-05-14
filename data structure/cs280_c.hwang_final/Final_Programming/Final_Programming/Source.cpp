#include "Astar.h"

int main(int argc, char **argv) {
	const char *test_num = nullptr;
	if (argc > 1) {
		test_num = argv[1];
	}

	if (test_num == nullptr) {
		Astar("level1.txt");
		Astar("level2.txt");
		Astar("level3.txt");
		Astar("level4.txt");
		Astar("level5.txt");
		Astar("level6.txt");
		Astar("level7a.txt");
		Astar("level7b.txt");
		Astar("level8.txt");
		Astar("level9.txt");
		Astar("level10.txt");
	}
	else {
		Astar(std::string("level") + argv[1] + ".txt");
	}
}