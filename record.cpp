#include "record.h"

std::string Record::getName() {
	return name;
}

std::string Record::getContent() {
	return content;
}

int Record::getTtl() {
	return ttl;
}

Tins::DNS::QueryType Record::getType() {
	return type;
}

int Record::getPriority() {
	return priority;
}

std::string Record::getMail() {
	return mail;
}

int Record::getSerial() {
	return serial;
}

int Record::getRefresh() {
	return refresh;
}

int Record::getRetry() {
	return retry;
}

int Record::getExpire() {
	return expire;
}

int Record::setName(std::string newName) {
	return 0;
}

int Record::setContent(std::string newContent) {
	return 0;
}

int Record::setTtl(int newTtl) {
	return 0;
}

int Record::setType(Tins::DNS::QueryType newType) {
	return 0;
}

int Record::setPriority(int newPriority) {
	priority = newPriority;
	return 0;
}

int Record::setMail(std::string newMail) {
	mail = newMail;
	return 0;
}

int Record::setSerial(int newSerial) {
	serial = newSerial;
	return 0;
}

int Record::setRefresh(int newRefresh) {
	refresh = newRefresh;
	return 0;
}

int Record::setRetry(int newRetry) {
	retry = newRetry;
	return 0;
}

int Record::setExpire(int newExpire) {
	expire = newExpire;
	return 0;
}

Record Record::returnCopy() {
	return Record(this->name, this->content, this->ttl, this->type);
}

Record::Record(std::string n, std::string c, int tt, Tins::DNS::QueryType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
}

Record::Record(std::string n, std::string c, int tt, int pri, Tins::DNS::QueryType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
	pri = priority;
}

Record::Record(std::string n, std::string c, int tt, std::string email, int ser, int ref, int ret, int exp, Tins::DNS::QueryType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
	mail = email;
	serial = ser;
	refresh = ref;
	retry = ret;
	expire = exp;
}

Record::~Record() {
}
