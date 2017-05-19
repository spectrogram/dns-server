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
#include <tuple>

#include "node.h"
#include "record.h"
#include "edge.h"
#include "trie.h"
#include "util.h"

extern Trie t;

std::vector<std::string> splitDomain(std::string domain);
std::string concatDomain(std::vector<std::string> domain);
std::tuple<int, int> findBestMatch(std::vector<std::unique_ptr<Edge>> const &v, std::string l, int index);
int compareFunc(std::string a, std::string b, int index);

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

	std::tuple<int, int> result = findBestMatch(nEdges, label, index);

	if (std::get<1>(result) == -1) {
		// no matches, so insert here
		// as it's the only node with this label, it's a leaf
		Node *newNode = new Node(label.length(), true);
		std::unique_ptr<Edge> newEdge(new Edge(label, newNode));
		newNode->addRecord(std::move(r));
		next.addEdge(std::move(newEdge));
		std::cout << "Memory location of newNode is: " << newNode << std::endl;
		return 0;
	}

	std::unique_ptr<Edge> &bestMatch = nEdges.at(std::get<1>(result));

	// we need to double check because... stuff
	if (label == bestMatch->getLabel()) {
		// full match, assume leaf?
		std::cout << "Inserting a record at node " << bestMatch->getTargetNode() << std::endl;
		bestMatch->getTargetNode()->addRecord(std::move(r));
		return 0;
	} else if (bestMatch->getLabel().length() > std::get<0>(result) || bestMatch->getTargetNode()->isLeaf()) {
		// split this node 
		std::cout << "Entered split (non leaf)" << std::endl;
		std::cout << "Result is " << std::get<1>(result) << std::endl;

		int newIndex = compareFunc(bestMatch->getLabel(), label, index);

		// create new node for prefix
		Node *newNodePrefix = new Node(newIndex, false);

		// create new edge from prefix node to target node
		// and add to prefix node
		std::string currEdgeName = bestMatch->getLabel().substr(0, newIndex);
		std::cout << currEdgeName << " is new prefix edge name with index " << newIndex << std::endl;
		std::unique_ptr<Edge> newEdge(new Edge(bestMatch->getLabel(), bestMatch->getTargetNode()));
		newNodePrefix->addEdge(std::move(newEdge));

		// wipe old edge from edges vector of current node
		std::vector<std::unique_ptr<Edge>>::iterator pos = nEdges.begin() + std::get<1>(result);
		std::cout << "Label of edge being removed is: " << (*pos)->getLabel() << std::endl;
		nEdges.erase(pos);
		//bestMatch.reset();

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
		std::cout << "New edge from curr node to prefix node is: " << newEdge3->getLabel() << std::endl;

		next.addEdge(std::move(newEdge3));
		std::cout << "Size of EDGES vector for next node: " << next.getEdges().size() << std::endl;
		// std::cout << "New edge from curr node to prefix node is: " << newEdge3->getLabel() << std::endl;
		for (std::vector<std::unique_ptr<Edge>>::const_iterator itn = next.getEdges().begin(); itn != next.getEdges().end(); itn++) {
			std::cout << "CURRENT EDGE NAME: " << (*itn)->getLabel() << std::endl;
		}

	} else {
		// recurse into the non-leaf node
		std::cout << "Recursing into non-leaf node " << bestMatch->getLabel() << std::endl;
		Node *n = bestMatch->getTargetNode();
		addRecord(std::move(r), (*n), std::get<0>(result));
	}

	return 0;
}

std::vector<Record> Trie::lookup(std::string search, Tins::DNS::QueryType type, Node & next, int index) {
	std::string label = concatDomain(splitDomain(search));
	std::vector<Record> results;

	std::cout << "The search label is " << label << std::endl;

	// first look for the matching node
	std::vector<std::unique_ptr<Edge>> &nEdges = next.getEdges();

	if (nEdges.empty()) {
		throw "Trie is empty!";
	}

	std::tuple<int, int> result = findBestMatch(nEdges, label, index);

	std::unique_ptr<Edge> &bestMatch = nEdges.at(std::get<1>(result));

	if (label == bestMatch->getLabel()) {
		// full match
		// loop through records for this node and get records that match record type
		std::vector<std::unique_ptr<Record>> r;
		bestMatch->getTargetNode()->getRecords();

		for (const auto& e : bestMatch->getTargetNode()->getRecords()) {
			r.push_back(util::make_unique<Record>(*e));
		}

		for (std::vector<std::unique_ptr<Record>>::const_iterator it = r.begin(); it != r.end(); it++) {
			if ((*it)->getType() == type) { 
				results.push_back((*it)->returnCopy());
			}
		}
		return results;
	} else if (!bestMatch->getTargetNode()->isLeaf()) {
		Node *n = bestMatch->getTargetNode();
		return lookup(search, type, (*n), std::get<0>(result));
	} else {
		// case to return best match if there is no exact match
		std::vector<std::unique_ptr<Record>> r; 
		bestMatch->getTargetNode()->getRecords();

		for (const auto& e : bestMatch->getTargetNode()->getRecords()) {
			r.push_back(util::make_unique<Record>(*e));
		}

		for (std::vector<std::unique_ptr<Record>>::const_iterator it = r.begin(); it != r.end(); it++) {
			if ((*it)->getType() == type) {
				results.push_back((*it)->returnCopy());
			}
		}
		return results;
	}

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
		std::cout << "Size of EDGES vector for current node: " << curr.getEdges().size() << std::endl;
		for (std::vector<std::unique_ptr<Edge>>::const_iterator itn = curr.getEdges().begin(); itn != curr.getEdges().end(); itn++) {
			std::cout << "CURRENT EDGE NAME: " << (*itn)->getLabel() << std::endl;
			Node *n = (*itn)->getTargetNode();
			std::cout << "Recursing to lower node" << std::endl;
			scanTrie(*n);
			std::cout << "Returning..." << std::endl;
		}
	}
}

int Trie::trimTrie(Node & curr) {
	if (curr.getEdges().size() < 2) {
		return 0;
	} else {
		std::sort(curr.getEdges().begin(), curr.getEdges().end());
		return 0;
	}
}
