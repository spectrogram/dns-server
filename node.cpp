#include "node.h"

bool Node::isLeaf()
{
	return false;
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

Node::Node(std::string n, std::string c, NodeType ty, int tt) {
	name = n;
	content = c;
	type = ty;
	ttl = tt;
}


Node::~Node() {
}
