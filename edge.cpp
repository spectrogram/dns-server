#include <string>
#include "edge.h"
#include "node.h"

Edge::Edge(std::string l, Node *n) {
	label = l;
	target = n;
}


Edge::~Edge() {

}

int Edge::setTargetNode(Node *n) {
	target = n;
	return 0;
}

std::string Edge::getLabel() {
	return label;
}

Node* Edge::getTargetNode() {
	return target;
}

//bool Edge::comparator(const Edge & a) {
//	return label < a.label;
//}
