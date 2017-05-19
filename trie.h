#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>
#include <memory>

class Edge;
class Record;
class Node;

class Trie {
public:
	Trie();
	~Trie();

	int addRecord(std::unique_ptr<Record> r, Node &next, int index);
	std::vector<Record> lookup(std::string search, Tins::DNS::QueryType type, Node & next, int index);
	int deleteRecord(Record *r);
	int updateRecord(Record *r);

	Node &getRoot();

	void scanTrie(Node &curr);
	int trimTrie(Node &curr);

private:
	Node* root;
};