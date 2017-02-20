#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "node.h"
#include "edge.h"

std::vector<std::string> splitDomain(Node n);
std::string concatDomain(std::vector<std::string> domain);

Node root;

Node createTrie() {
	std::string n = ".";
	std::string c = ".";
	int ttl = 36000;
	NodeType ty = ZoneAuth;

	root = Node(n, c, ttl, ty);

	// root.printNode();

	return root;
}

// TODO:
// This will not work for NS/SOA records. Need 2 separate cases for records and ZoneAuth
// ZoneAuth reside on the 
int insertIntoTrie(Node &n, Node &next, int index) {
	std::string label = concatDomain(splitDomain(n));
	std::cout << "Memory location of root is: " << &next << std::endl;
	std::cout << "Memory location of next is: " << &n << std::endl;
	std::cout << "Label is " << label << std::endl;

	std::vector<Edge> nEdges = next.getEdges();

	for (std::vector<Edge>::iterator it = nEdges.begin(); it != nEdges.end(); it++) {
		std::string targetName = it->getLabel();
		
		for (int i = 0; i < targetName.length(); ++i) {
			std::cout << "About to compare " << targetName[i] << " and " << label[i] << std::endl;
			
			if (targetName.compare(label) == 0) {
				std::cout << "Exact match " << targetName << " and " << label << std::endl;
				std::cout << it->getLabel() << std::endl;
				std::cout << "Size of itt is: " << sizeof(it->getTargetNode()) << std::endl;

				// this is an exact match so we'll put it here
				if (it->getTargetNode().isIndex()) {
					Node match = it->getTargetNode();
					insertIntoTrie(n, match, index);
					// how to use index????
				} else {
					// we need to turn the existing record node into an index node
					Node newNode = Node();
					newNode.setIfIndex(true);
					newNode.setIndex(label.length());
					
					// move old node so it's a child of the new node
					Node old = it->getTargetNode();
					Edge 

					it->setTargetNode(newNode);
					break;
				}

			} else if ((targetName[i] != label[i]) || (targetName[i] == label[i] && targetName[i + 1] == label[i + 1])) {
				// otherwise loop through all non matching and currently matching labels
				it++;
			}
		}
	}
	
	// if no matches, insert here
	Edge newEdge = Edge(label);
	newEdge.setTargetNode(n);
	next.addEdge(newEdge);

	std::cout << "Target node of new edge is: " << newEdge.getTargetNode().getName() << std::endl;
	return 0;


}