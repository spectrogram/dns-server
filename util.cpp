#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <tuple>

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

int compareFunc(std::string a, std::string b, int index) {
	int len;
	// std::cout << "We are comparing " << a << " and " << b << " from index " << index << std::endl;

	len = std::min(a.length(), b.length());

	int j = 0;
	for (int i = index; i < len; i++) {
		if (a[i] == b[i]) {
			j++;
			continue;
		} else {
			if (j > 0) {
				// std::cout << "Returning " << j + index << std::endl;
				return j + index;
			} else {
				return 0;
			}
		}
	}

	return len;
}



std::tuple<int, int> findBestMatch(std::vector<std::unique_ptr<Edge>> const &v, std::string l, int index) {
	int pos = -1;
	int currMax = 0;

	for (std::vector<std::unique_ptr<Edge>>::const_iterator it = v.begin(); it != v.end(); it++) {
		std::string currLabel = (*it)->getLabel();
		int comp = compareFunc(currLabel, l, index);
		if (comp > currMax) {
			currMax = comp;
			// std::cout << "currmax is " << currMax << std::endl;
			pos = it - v.begin();
		}
	}
	// std::cout << "I'm returning " << pos << std::endl;
	return std::make_tuple(currMax, pos);
}

//std::string longestCommonSubstring(std::vector<std::unique_ptr<Edge>> const &v) {
//
//}