#include "soa.h"

SOA::~SOA() {
}

std::string SOA::getEmail() {
	return email;
}

int SOA::getSerial() {
	return serial;
}

int SOA::getRefresh() {
	return refresh;
}

int SOA::getRetry() {
	return retry;
}

int SOA::getExpire() {
	return expire;
}

int SOA::setEmail(std::string newEmail) {
	email = newEmail;
	return 0;
}

int SOA::setSerial(int newSerial) {
	serial = newSerial;
	return 0;
}

int SOA::setRefresh(int newRefresh) {
	refresh = newRefresh;
	return 0;
}

int SOA::setRetry(int newRetry) {
	retry = newRetry;
	return 0;
}

int SOA::setExpire(int newExpire) {
	expire = newExpire;
	return 0;
}
