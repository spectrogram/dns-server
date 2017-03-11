#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "node.h"
#include "edge.h"

bool Node::isLeaf() {
	return leaf;
}

int Node::getIndex() {
	return index;
}

std::vector<std::unique_ptr<Edge>> &Node::getEdges() {
	return edges;
}

std::vector<std::unique_ptr<Record>> &Node::getRecords() {
	return records;
}

int Node::addEdge(std::unique_ptr<Edge> e) {
	edges.push_back(std::move(e));
	return 0;
}

int Node::addRecord(std::unique_ptr<Record> r) {
	records.push_back(std::move(r));
	std::cout << "Size of records vector is now: " << records.size() << std::endl;

	for (std::vector<std::unique_ptr<Record>>::iterator it2 = records.begin(); it2 != records.end(); it2++) {
		std::cout << "CURRENT RECORD NAME: " << (*it2)->getName() << std::endl;
		std::cout << "CURRENT CONTENT NAME: " << (*it2)->getContent() << std::endl;
		std::cout << "CURRENT RECORD TYPE: " << (*it2)->getType() << std::endl;
	}

	return 0;
}

Node::Node(int i, bool l) {
	index = i;
	leaf = l;
}

Node::Node() {
}

Node::~Node() {
}
