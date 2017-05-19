#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>
#include <tins/tins.h>

class Record {
public:
	std::string getName();
	std::string getContent();
	int getTtl();
	Tins::DNS::QueryType getType();

	int setName(std::string newName);
	int setContent(std::string newContent);
	int setTtl(int newTtl);
	int setType(Tins::DNS::QueryType newType);

	Record returnCopy();

	Record(std::string n, std::string c, int ttl , Tins::DNS::QueryType rec);
	~Record();

protected:
	std::string name;
	std::string content;
	int ttl;
	Tins::DNS::QueryType type;

};

