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

protected:
	int priority;
	int weight;
	int port;

};