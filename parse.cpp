#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "split.cpp"
#include "node.h"
#include "trie.cpp"

int scanFile(std::string path) {
	std::ifstream input(path.c_str());
	std::cout << path;

	std::string currLine;
	while (std::getline(input, currLine)) {
		std::vector<std::string> record;

		record = split(currLine, ' ');

		std::string n = record[0].substr(0, record[0].size() - 1);
		std::string c = record[4];
		int tt = atoi(record[1].c_str());
		std::cout << n << std::endl;

		if (record[3].compare("NS") == 0) {
			std::cout << "NAMESERVER" << currLine << std::endl;
			NodeType ty = ZoneAuth;
			Node newNode = Node(n, c, ty, tt);
			insertIntoTrie(newNode);
		}

		if (record[3].compare("A") == 0) {
			std::cout << "A RECORD" << currLine << std::endl;
			NodeType ty = Record;
			Node newNode = Node(n, c, ty, tt);
		}

		//for (std::vector<std::string>::iterator it = record.begin(); it != record.end(); ++it) {
		//	std::cout << *it << std::endl;
		//}
	}

	return 0;
}

int splitDomain(Node n) {
	std::string domain = n.getName();


}