#include "srv.h"

int SRV::getPriority() {
	return priority;
}

int SRV::getWeight() {
	return weight;
}

int SRV::getPort() {
	return port;
}

int SRV::setPriority(int newPriority) {
	priority = newPriority;
	return 0;
}

int SRV::setWeight(int newWeight) {
	weight = newWeight;
	return 0;
}

int SRV::setPort(int newPort) {
	port = newPort;
	return 0;
}

SRV::~SRV() {
}
