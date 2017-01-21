#pragma once
#include "node.h"

class Edge {
public:
	Edge(std::string label);
	~Edge();

	std::string getLabel();
	Node getTargetNode();

private:
	std::string label;
	Node target;

};

