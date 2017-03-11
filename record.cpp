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

RecordType Record::getType() {
	return type;
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

int Record::setType(RecordType newType) {
	return 0;
}

Record::Record(std::string n, std::string c, int tt, RecordType rec) {
	name = n;
	content = c;
	ttl = tt;
	type = rec;
}

Record::~Record() {
}
