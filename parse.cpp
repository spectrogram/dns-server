#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "node.h"
#include "trie.h"
#include "record.h"
#include "edge.h"

extern Trie t;

int scanFile(std::string path);
std::vector<std::string> splitDomain(std::string domain);
std::vector<std::string> split(const std::string &s, char delim);
std::string concatDomain(std::vector<std::string> domain);

int scanFile(std::string path) {
	std::ifstream input(path.c_str());

	std::string currLine;

	Record *temp;
	while (std::getline(input, currLine)) {
		std::vector<std::string> record;

		record = split(currLine, ' ');

		std::string n = record[0].substr(0, record[0].size() - 1);
		std::string c = record[4];
		int tt = atoi(record[1].c_str());

		if (record[3].compare("A") == 0) {
			std::unique_ptr<Record> newRecord (new Record(n, c, tt, A));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		if (record[3].compare("NS") == 0) {
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, NS));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}


	}

	return 0;
}

