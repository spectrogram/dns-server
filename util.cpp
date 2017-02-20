#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "node.h"
#include "edge.h"
#include "root.h"

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

std::vector<std::string> splitDomain(Node n) {
	std::string domain = n.getName();

	std::vector<std::string> zones;

	zones = split(domain, '.');

	std::reverse(zones.begin(), zones.end());

	//for (std::vector<std::string>::const_iterator i = zones.begin(); i != zones.end(); ++i) {
	//	std::cout << *i << std::endl;
	//}

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

void scanTrie(Node &n) {
	std::cout << "CURRENT NODE: " << n.getName() << std::endl;

	std::vector<Edge> edges = n.getEdges();

	std::cout << "Size of edges vector: " << edges.size() << std::endl;

	for (std::vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
		std::cout << "CURRENT EDGE NAME: " << it->getLabel() << std::endl;
		if (it->getTargetNode().isIndex()) {
			Node next = it->getTargetNode();
			scanTrie(next);
		}
	}
}