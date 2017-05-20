#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <typeinfo>

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

		if (record.size() < 4) {
			return 1;
		}

		std::string n = record[0].substr(0, record[0].size() - 1);
		std::string c = record[4];
		uint32_t tt = std::strtoul(record[1].c_str(), NULL, 0);

		if (record[3].compare("A") == 0) {
			std::unique_ptr<Record> newRecord (new Record(n, c, tt, Tins::DNS::A));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		if (record[3].compare("AAAA") == 0) {
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, Tins::DNS::AAAA));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		if (record[3].compare("NS") == 0) {
			if (c.back() == '.') {
				c = c.substr(0, c.size() - 1);
			}
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, Tins::DNS::NS));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		if (record[3].compare("CNAME") == 0) {
			if (c.back() == '.') {
				c = c.substr(0, c.size() - 1);
			}
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, Tins::DNS::CNAME));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		if (record[3].compare("TXT") == 0) {
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, Tins::DNS::TXT));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		if (record[3].compare("MX") == 0) {
			c = record[5];
			if (c.back() == '.') {
				c = c.substr(0, c.size() - 1);
			}
			uint16_t priority = atoi(record[4].c_str());
			std::cout << "MX Priority read in as " << priority << std::endl;
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, priority, Tins::DNS::MX));
			std::cout << "MX Priority comes out as " << newRecord->getPriority() << std::endl;
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		//if (record[3].compare("SRV") == 0) {
		//	c = record[7];
		//	int priority = atoi(record[4].c_str());
		//	int weight = atoi(record[5].c_str());
		//	int port = atoi(record[6].c_str());
		//	std::unique_ptr<Record> newRecord(new Record(n, c, tt, priority, weight, port, Tins::DNS::SRV));
		//	t.addRecord(std::move(newRecord), t.getRoot(), 0);
		//}

		if (record[3].compare("SOA") == 0) {
			c = record[4];
			if (c.back() == '.') {
				c = c.substr(0, c.size() - 1);
			}

			std::string mail = record[5];
			if (mail.back() == '.') {
				mail = mail.substr(0, mail.size() - 1);
			}

			uint32_t serial = std::strtoul(record[6].c_str(), NULL, 0);
			uint32_t refresh = std::strtoul(record[7].c_str(), NULL, 0);
			uint32_t retry = std::strtoul(record[8].c_str(), NULL, 0);
			uint32_t expire = std::strtoul(record[9].c_str(), NULL, 0);
			uint32_t minttl = std::strtoul(record[10].c_str(), NULL, 0);
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, mail, serial, refresh, retry, expire, minttl, Tins::DNS::SOA));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}
	}

	return 0;
}

