#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>

#include "record.h"

class SRV : public Record {
public:
	int getPriority();
	int getWeight();
	int getPort();

	int setPriority(int newPriority);
	int setWeight(int newWeight);
	int setPort(int newPort);

	SRV(std::string n, std::string c, int ttl, int priority, int weight, int port, Tins::DNS::QueryType rec) : Record(n, c, ttl, rec) {};
	~SRV();

protected:
	int priority;
	int weight;
	int port;

};