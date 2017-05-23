#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <typeinfo>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "node.h"
#include "trie.h"
#include "record.h"
#include "edge.h"

extern Trie t;

int scanFile(std::string path);
int scanZoneFile(std::string path);
std::vector<std::string> splitDomain(std::string domain);
std::vector<std::string> split(const std::string &s, char delim);
std::string concatDomain(std::vector<std::string> domain);


int scanFile(std::string path) {
	std::ifstream input(path.c_str());

	std::string currLine;

	std::vector<std::string> record;
	std::cout << currLine << "\n";
	while (std::getline(input, currLine)) {

		try {
			boost::tokenizer<boost::escaped_list_separator<char>> tok(currLine, boost::escaped_list_separator<char>('\\', ' ', '\"'));
			record.assign(tok.begin(), tok.end());
		} catch (boost::escaped_list_error e) {
			continue;
		}

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
			std::unique_ptr<Record> newRecord(new Record(n, c, tt, priority, Tins::DNS::MX));
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


// special function for performance testing - load Verisign's com/net rrset
int scanZoneFile(std::string path) {
	char buffer[16384];
	std::ifstream input(path.c_str());
	input.rdbuf()->pubsetbuf(buffer, 16384);

	std::string currLine;
	
	std::vector<std::string> record;
	std::string n;
	std::string c;
	while (std::getline(input, currLine)) {
		boost::split(record, currLine, boost::is_any_of(" "));

		if (record.size() > 3) {
			continue;
		}

		// these are A records for nameservers
		if (record[1] == "A") {
			n = record[0];
			n.append(".NET");

			c = record[2];

			std::unique_ptr<Record> newRecord(new Record(n, c, 86400, Tins::DNS::A));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		// these are AAAA record for nameservers
		if (record[1] == "AAAA") {
			n = record[0];
			n.append(".NET");

			c = record[2];

			std::unique_ptr<Record> newRecord(new Record(n, c, 86400, Tins::DNS::AAAA));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}

		// these are NS records 
		if (record[1] == "NS") {
			n = record[0];
			n.append(".NET");

			c = record[2];
			boost::trim_right(c);
			if (c.back() != '.') {
				c.append(".NET");
			}

			std::unique_ptr<Record> newRecord(new Record(n, c, 86400, Tins::DNS::NS));
			t.addRecord(std::move(newRecord), t.getRoot(), 0);
		}
		
		record.clear();
		std::vector<std::string>().swap(record);
	}
	std::cout << "Finished.\n";
	return 0;
}