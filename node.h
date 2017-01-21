#pragma once
#include <string>
#include <vector>

class Edge;

enum NodeType { None, Record, ZoneAuth };

class Node {
public:

	bool isLeaf();
	std::string getName();
	std::string getContent();
	NodeType getType();
	int getTtl();

	Node(std::string n, std::string c, NodeType ty, int tt);
	~Node();

private:
	std::vector<Edge> edges;

	NodeType type;
	std::string name;
	std::string content;
	int ttl;
};

