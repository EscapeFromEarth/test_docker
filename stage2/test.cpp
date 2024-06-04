#include <iostream>
#include <string>
#include "proto/test.pb.h"

int main() {
	while (true) {
		Test::Child oChild;
		uint32_t iAge = 0, iToyNum = 0;
		std::cin >> iAge >> iToyNum;
		oChild.set_age(iAge);
		for (int i = 0; i < iToyNum; i++) {
			auto *pToy = oChild.add_toy();
			uint32_t iColor = 0;
			std::string sDesc;
			std::cin >> iColor >> sDesc;
			pToy->set_color(iColor);
			pToy->set_desc(sDesc);
		}
		std::cout << oChild.DebugString() << std::endl;
	}
	
	return 0;
}