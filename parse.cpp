#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "node.h"
#include "edge.h"
#include "root.h"

int scanFile(std::string path);
std::vector<std::string> splitDomain(Node n);
std::vector<std::string> split(const std::string &s, char delim);
int insertIntoTrie(Node &n, Node &next, int index);
std::string concatDomain(std::vector<std::string> domain);

int scanFile(std::string path) {
	std::ifstream input(path.c_str());

	std::string currLine;
	while (std::getline(input, currLine)) {
		std::vector<std::string> record;

		std::cout << currLine << std::endl;

		record = split(currLine, ' ');

		std::string n = record[0].substr(0, record[0].size() - 1);
		std::string c = record[4];
		int tt = atoi(record[1].c_str());
		// std::cout << n << std::endl;

		Node newNode = Node();
		newNode.setName(n);
		newNode.setContent(c);
		newNode.setTtl(tt);
		

		if (record[3].compare("NS") == 0) {
			// std::cout << "NAMESERVER" << currLine << std::endl;
			NodeType ty = ZoneAuth;
			newNode.setIfIndex(true);
			newNode.setType(ty);
			insertIntoTrie(newNode, root, 0);
			std::cout << "INSERTED" << std::endl;
		}

		//if (record[3].compare("MX") == 0) {
		//	std::cout << "MX RECORD" << currLine << std::endl;
		//	NodeType ty = Record;
		//	newNode.setIfIndex(false);
		//	newNode.setType(ty);
		//	insertIntoTrie(newNode, root, 0);
		//	std::cout << "INSERTED" << std::endl;
		//}

		//for (std::vector<std::string>::iterator it = record.begin(); it != record.end(); ++it) {
		//	std::cout << *it << std::endl;
		//}
	}

	return 0;
}

