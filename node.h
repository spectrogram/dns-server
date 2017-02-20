#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>

class Edge;

enum NodeType { None, Record, ZoneAuth };

class Node {
public:
	int setName(std::string n);
	int setContent(std::string c);
	int setType(NodeType ty);
	int setTtl(int time);
	int setPriority(int p);

	bool isIndex();
	int getIndex();
	int setIndex(int i);
	bool setIfIndex(bool ind);

	bool isLeaf();

	int addEdge(Edge e);

	std::string getName();
	std::string getContent();
	NodeType getType();
	int getTtl();
	std::vector<Edge> getEdges();
	int getPriority();
	void printNode() const;

	bool comparator(const Node & a);

	Node();
	Node(std::string n, std::string c, int tt, NodeType ty);
	~Node();

private:
	std::vector<Edge> edges;

	NodeType type;
	std::string name;
	std::string content;
	int ttl;
	int priority;

	bool index;
	int indexNum;
};

