#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <typeinfo>
#include <stdio.h>

#include "node.h"
#include "record.h"
#include "edge.h"
#include "trie.h"
#include "compareStrings.h"

extern Trie t;

std::vector<std::string> splitDomain(std::string domain);
std::string concatDomain(std::vector<std::string> domain);

Trie::Trie() {
	root = new Node(0, false);
}

Trie::~Trie() {
}

Node & Trie::getRoot() {
	return *root;
}

int Trie::addRecord(std::unique_ptr<Record> r, Node & next, int index) {
	std::string label = concatDomain(splitDomain(r->getName()));
	std::cout << "Label is " << label << std::endl;

	std::vector<std::unique_ptr<Edge>> &nEdges = next.getEdges();

	if (nEdges.empty()) {
		// if trie completely empty, create a new edge and node
		Node *newNode = new Node(label.length(), true);
		std::unique_ptr<Edge> newEdge(new Edge(label, newNode));
		newNode->addRecord(std::move(r));
		next.addEdge(std::move(newEdge));
		std::cout << "Inserted a new edge and node at " << newNode << std::endl;
		return 0;
	}

	std::vector<std::unique_ptr<Edge>>::iterator bestMatch = std::max_element(nEdges.begin(), nEdges.end(), compareStrings(index));

	int result = compareFunc(label, (*bestMatch)->getLabel(), index);

	// we need to double check because... stuff
	if (result == 0) {
		// no matches, so insert here
		// as it's the only node with this label, it's a leaf
		Node *newNode = new Node(label.length(), true);
		std::unique_ptr<Edge> newEdge(new Edge(label, newNode));
		newNode->addRecord(std::move(r));
		next.addEdge(std::move(newEdge));
		std::cout << "Memory location of newNode is: " << newNode << std::endl;
		return 0;
	} else if (label == (*bestMatch)->getLabel()) {
		// full match, assume leaf?
		std::cout << "Inserting a record at node " << (*bestMatch)->getTargetNode() << std::endl;
		(*bestMatch)->getTargetNode()->addRecord(std::move(r));
		return 0;
	} else if ((*bestMatch)->getTargetNode()->isLeaf()) {
			// existing node needs to be under a prefix shared by new node 
			// we need to split the existing node
			std::cout << "Entered split " << std::endl;
			std::cout << "Result is " << result << std::endl;

			// create new node for prefix
			Node *newNodePrefix = new Node(result, false);
			std::string currEdgeName = (*bestMatch)->getLabel().substr(0, result);
			std::cout << currEdgeName << " is new prefix edge name" << std::endl;

			// create new edge from prefix node to target node
			// and add to prefix node
			std::unique_ptr<Edge> newEdge(new Edge((*bestMatch)->getLabel(), (*bestMatch)->getTargetNode()));
			newNodePrefix->addEdge(std::move(newEdge));

			// wipe old edge from edges vector of current node
			nEdges.erase(bestMatch);
			(*bestMatch).reset();
			
			// create new node for records we're currently trying to put in
			// and insert the records too...
			Node *newNode = new Node(label.length(), true);
			newNode->addRecord(std::move(r));

			// add edge from prefix node to new node
			// and add to current node
			std::unique_ptr<Edge> newEdge2(new Edge(label, newNode));
			newNodePrefix->addEdge(std::move(newEdge2));

			// finally, create new edge from current node to prefix node
			std::unique_ptr<Edge> newEdge3(new Edge(currEdgeName, newNodePrefix));
			next.addEdge(std::move(newEdge3));
			
		} else {
			// recurse into the non-leaf node
			std::cout << "Entered this bit" << std::endl;
			Node *n = (*bestMatch)->getTargetNode();
			addRecord(std::move(r), (*n), result);
		}

	return 0;
}

void Trie::scanTrie(Node &curr) {
	std::cout << "Size of records vector for current node: " << curr.getRecords().size() << std::endl;
	std::cout << "Index of current node: " << curr.getIndex() << std::endl;
	for (std::vector<std::unique_ptr<Record>>::const_iterator itr = curr.getRecords().begin(); itr != curr.getRecords().end(); itr++) {
		std::cout << "CURRENT RECORD NAME: " << (*itr)->getName() << std::endl;
		std::cout << "CURRENT CONTENT NAME: " << (*itr)->getContent() << std::endl;
		std::cout << "CURRENT RECORD TYPE: " << (*itr)->getType() << std::endl;
	}

	if (!curr.isLeaf()) {
		for (std::vector<std::unique_ptr<Edge>>::const_iterator itn = curr.getEdges().begin(); itn != curr.getEdges().end(); itn++) {
			Node *n = (*itn)->getTargetNode();
			std::cout << "CURRENT EDGE NAME: " << (*itn)->getLabel() << std::endl;
			std::cout << "Recursing to lower node" << std::endl;
			scanTrie(*n);
			std::cout << "Returning..." << std::endl;
		}
	}
}

