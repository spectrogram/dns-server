#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>
#include <memory>

#include "record.h"
class Edge;

class Node {
public:
	bool isLeaf();
	int getIndex();
	std::vector<std::unique_ptr<Edge>> &getEdges();
	std::vector<std::unique_ptr<Record>> &getRecords();

	int setIndex();
	int addEdge(std::unique_ptr<Edge> e);
	int addRecord(std::unique_ptr<Record> r);

	Node(int i, bool l);
	Node();
	~Node();

protected:
	bool leaf;
	int index;
	std::vector<std::unique_ptr<Edge>> edges;
	std::vector<std::unique_ptr<Record>> records;
};

