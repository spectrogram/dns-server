#include <string>
#include "edge.h"
#include "node.h"

Edge::Edge(std::string l) {
	label = l;
}


Edge::~Edge() {

}

int Edge::setTargetNode(Node &n) {
	target = &n;
}

std::string Edge::getLabel() {
	return label;
}

Node Edge::getTargetNode() {
	return *target;
}

//bool Edge::comparator(const Edge & a) {
//	return label < a.label;
//}
