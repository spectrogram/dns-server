#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>

enum RecordType { A, AAAA, CNAME, TXT, NS, SRV, MX, SOA };

class Record {
public:
	std::string getName();
	std::string getContent();
	int getTtl();
	RecordType getType();

	int setName(std::string newName);
	int setContent(std::string newContent);
	int setTtl(int newTtl);
	int setType(RecordType newType);

	Record returnCopy();

	Record(std::string n, std::string c, int ttl , RecordType rec);
	~Record();

protected:
	std::string name;
	std::string content;
	int ttl;
	RecordType type;

};

