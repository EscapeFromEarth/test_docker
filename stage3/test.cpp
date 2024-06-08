#include "iostream"

#include "unistd.h"

// 需要先 cmake -S . -B build; cmake --build build

int main() {
	for (int i = 0; ; i++, sleep(1)) {
		std::cout << "Hello World #" << i << std::endl;
	}
	
	return 0;
}