#pragma once

#include "node.h"

class Edge {
public:
	Edge(std::string label, Node *n);
	~Edge();

	int setTargetNode(Node *n);

	std::string getLabel();
	Node *getTargetNode();

	bool operator < (const Edge &b) const { return label < b.label;  }

	// bool comparator(const Edge & a);

private:
	std::string label;
	Node *target;

};

