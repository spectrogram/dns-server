#include "record.h"

std::string Record::getName() {
	return name;
}

std::string Record::getContent() {
	return content;
}

uint32_t Record::getTtl() {
	return ttl;
}

Tins::DNS::QueryType Record::getType() {
	return type;
}

uint16_t Record::getPriority() {
	return priority;
}

std::string Record::getMail() {
	return mail;
}

uint32_t Record::getSerial() {
	return serial;
}

uint32_t Record::getRefresh() {
	return refresh;
}

uint32_t Record::getRetry() {
	return retry;
}

uint32_t Record::getExpire() {
	return expire;
}

uint32_t Record::getMinTtl() {
	return minTtl;
}

int Record::setName(std::string newName) {
	return 0;
}

int Record::setContent(std::string newContent) {
	return 0;
}

int Record::setTtl(uint32_t newTtl) {
	return 0;
}

int Record::setType(Tins::DNS::QueryType newType) {
	return 0;
}

int Record::setPriority(uint16_t newPriority) {
	priority = newPriority;
	return 0;
}

int Record::setMail(std::string newMail) {
	mail = newMail;
	return 0;
}

int Record::setSerial(uint32_t newSerial) {
	serial = newSerial;
	return 0;
}

int Record::setRefresh(uint32_t newRefresh) {
	refresh = newRefresh;
	return 0;
}

int Record::setRetry(uint32_t newRetry) {
	retry = newRetry;
	return 0;
}

int Record::setExpire(uint32_t newExpire) {
	expire = newExpire;
	return 0;
}

int Record::setMinTtl(uint32_t newMinTtl) {
	minTtl = newMinTtl;
	return 0;
}

Record Record::returnCopy() {
	return Record(this->name, this->content, this->ttl, this->type);
}

Record Record::returnSOACopy() {
	return Record(this->name, this->content, this->ttl, this->mail, this->serial, this->refresh, this->retry, this->expire, this->minTtl, this->type);
}

Record Record::returnMXCopy() {
	return Record(this->name, this->content, this->ttl, this->priority, this->type);
}

Record::Record(std::string n, std::string c, uint32_t tt, Tins::DNS::QueryType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
}

Record::Record(std::string n, std::string c, uint32_t tt, uint16_t pri, Tins::DNS::QueryType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
	priority = pri;
}

Record::Record(std::string n, std::string c, uint32_t tt, std::string email, uint32_t ser, uint32_t ref, uint32_t ret, uint32_t exp, uint32_t min, Tins::DNS::QueryType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
	mail = email;
	serial = ser;
	refresh = ref;
	retry = ret;
	expire = exp;
	minTtl = min;
}

Record::~Record() {
}
