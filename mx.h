#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>

#include "record.h"
class MX : public Record {
public:
	int getPriority() override { return priority; };

	int setPriority(int newPriority);

	MX(std::string n, std::string c, int ttl, int priority, Tins::DNS::QueryType rec) : Record(n, c, ttl, rec) {};
	~MX();

protected:
	int priority;

};
