#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include "parse.cpp"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " path" << std::endl;
		return 1;
	}

	scanFile(argv[1]);

	return 0;
}