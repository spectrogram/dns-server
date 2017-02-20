#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include "node.h"
#include "root.h"

Node createTrie();
int scanFile(std::string path);
void scanTrie(Node &n);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " path" << std::endl;
		return 1;
	}

	createTrie();

	scanFile(argv[1]);

	scanTrie(root);

	return 0;
}