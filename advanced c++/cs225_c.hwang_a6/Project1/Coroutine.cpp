#include <windows.h>
#include <algorithm>
#include <mutex>

int var;
std::mutex global_m;
extern void very_slow_function(int);

generator<int> locked_increment()
{
	std::lock_guard grab(global_m);
	var++;
	co_yield 1;
}