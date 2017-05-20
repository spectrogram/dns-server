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
	// mx and soa
	int getPriority();
	std::string getMail();
	int getSerial();
	int getRefresh();
	int getRetry();
	int getExpire();

	int setName(std::string newName);
	int setContent(std::string newContent);
	int setTtl(int newTtl);
	int setType(Tins::DNS::QueryType newType);
	// mx and soa
	int setPriority(int newPriority);
	int setMail(std::string newMail);
	int setSerial(int newSerial);
	int setRefresh(int newRefresh);
	int setRetry(int newRetry);
	int setExpire(int newExpire);

	Record returnCopy();

	// constructor for standard types
	Record(std::string n, std::string c, int tt , Tins::DNS::QueryType rec);
	// constructor for mx
	Record(std::string n, std::string c, int tt, int pri, Tins::DNS::QueryType rec);
	// constructor for soa
	Record(std::string n, std::string c, int tt, std::string email, int ser, int ref, int ret, int exp, Tins::DNS::QueryType rec);

	~Record();

protected:
	std::string name;
	std::string content;
	int ttl;
	Tins::DNS::QueryType type;

	// priority for mx and srv records
	int priority;

	// mail addr, serial, refresh, retry, expire for soa
	std::string mail;
	int serial;
	int refresh;
	int retry;
	int expire;

};

