#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>

#include "node.h"
#include "record.h"
#include "edge.h"

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::vector<std::string> splitDomain(std::string domain) {

	std::vector<std::string> zones;

	zones = split(domain, '.');

	std::reverse(zones.begin(), zones.end());

	return zones;
}

std::string concatDomain(std::vector<std::string> domain) {
	std::string s;

	for (std::vector<std::string>::const_iterator i = domain.begin(); i != domain.end(); ++i) {
		s += *i;
		s += "|";
	}

	s = s.substr(0, s.size() - 1);
	return s;
}
