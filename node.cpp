#include <string>
#include <iostream>

#include "node.h"
#include "edge.h"

int Node::setName(std::string n) {
	name = n;
	return 0;
}

int Node::setContent(std::string c) {
	content = c;
	return 0;
}

int Node::setType(NodeType ty) {
	type = ty;
	return 0;
}

int Node::setTtl(int time) {
	ttl = time;
	return 0;
}

int Node::setPriority(int p) {
	priority = p;
	return 0;
}

bool Node::isLeaf() {
	return false;
}

int Node::addEdge(Edge e) {
	

	edges.push_back(e);
	// std::sort(edges.begin(), edges.end());

	return 0;
}

bool Node::isIndex() {
	return index;
}

int Node::getIndex() {
	return indexNum;
}

int Node::setIndex(int i) {
	indexNum = i;
	return 0;
}

bool Node::setIfIndex(bool ind) {
	index = ind;
}

std::string Node::getName() {
	return name;
}

std::string Node::getContent() {
	return content;
}

NodeType Node::getType() {
	return type;
}

int Node::getTtl() {
	return ttl;
}

std::vector<Edge> Node::getEdges() {
	return edges;
}

int Node::getPriority() {
	return priority;
}

void Node::printNode() const {
	std::cout << "Name:" << name << std::endl;
	std::cout << "Content:" << content << std::endl;
	std::cout << "Type:" << type << std::endl;
}

bool Node::comparator(const Node & a) {
	return name < a.name;
}

Node::Node() {
}

Node::Node(std::string n, std::string c, int tt, NodeType ty) {
	name = n;
	content = c;
	ttl = tt;
	type = ty;
}

Node::~Node() {
}
