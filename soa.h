#pragma once
#include <string>
#include <vector>
#include <stdbool.h>
#include <algorithm>

#include "record.h"

class SOA : public Record {
public:

	std::string getEmail();
	int getSerial();
	int getRefresh();
	int getRetry();
	int getExpire();

	int setEmail(std::string newEmail);
	int setSerial(int newSerial);
	int setRefresh(int newRefresh);
	int setRetry(int newRetry);
	int setExpire(int newExpire);


	// content field is primary master, ttl field is the soa min ttl
	SOA(std::string n, std::string c, int ttl, std::string mail, int serial, int refresh, int retry, int expire, Tins::DNS::QueryType rec) : Record(n, c, ttl, rec) {};
	~SOA();

protected:
	std::string primary;
	std::string email;
	int serial;
	int refresh;
	int retry;
	int expire;
};