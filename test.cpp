#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include "node.h"
#include "trie.h"
#include "record.h"

Trie t = Trie();

int scanFile(std::string path);
int compareStrings(std::string a, std::string b);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " path" << std::endl;
		return 1;
	}

	scanFile(argv[1]);

	t.scanTrie(t.getRoot());	

	return 0;
}