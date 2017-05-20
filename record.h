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
	uint32_t getTtl();
	Tins::DNS::QueryType getType();
	// mx and soa
	uint16_t getPriority();
	std::string getMail();
	uint32_t getSerial();
	uint32_t getRefresh();
	uint32_t getRetry();
	uint32_t getExpire();
	uint32_t getMinTtl();

	int setName(std::string newName);
	int setContent(std::string newContent);
	int setTtl(uint32_t newTtl);
	int setType(Tins::DNS::QueryType newType);
	// mx and soa
	int setPriority(uint16_t newPriority);
	int setMail(std::string newMail);
	int setSerial(uint32_t newSerial);
	int setRefresh(uint32_t newRefresh);
	int setRetry(uint32_t newRetry);
	int setExpire(uint32_t newExpire);
	int setMinTtl(uint32_t newMinTtl);

	Record returnCopy();

	Record returnSOACopy();

	Record returnMXCopy();

	// constructor for standard types
	Record(std::string n, std::string c, uint32_t tt , Tins::DNS::QueryType rec);
	// constructor for mx
	Record(std::string n, std::string c, uint32_t tt, uint16_t pri, Tins::DNS::QueryType rec);
	// constructor for soa
	Record(std::string n, std::string c, uint32_t tt, std::string email, uint32_t ser, uint32_t ref, uint32_t ret, uint32_t exp, uint32_t min, Tins::DNS::QueryType rec);

	~Record();

protected:
	std::string name;
	std::string content;
	uint32_t ttl;
	Tins::DNS::QueryType type;

	// priority for mx and srv records
	uint16_t priority;

	// mail addr, serial, refresh, retry, expire for soa
	std::string mail;
	uint32_t serial;
	uint32_t refresh;
	uint32_t retry;
	uint32_t expire;
	uint32_t minTtl;

};

