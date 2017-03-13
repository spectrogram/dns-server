#pragma once
#include <iostream>
#include <algorithm>
#include <memory>

#include "edge.h"

int compareFunc(std::string a, std::string b, int index) {
	int len;
	std::cout << "We are comparing " << a << " and " << b << " from index " << index << std::endl;

	len = std::min(a.length(), b.length());

	int j = 0;
	for (int i = index; i < len; i++) {
		if (a[i] == b[i]) {
			j++;
			continue;
		} else {
			std::cout << "Returning " << j << std::endl;
			return j;
		}
	}

	return len;
}

class compareStrings {
	int index;
public:
	compareStrings(int i) {
		index = i;
	}

	int operator()(std::unique_ptr<Edge> const &a, std::unique_ptr<Edge> const &b) {
		return compareFunc(a->getLabel(), b->getLabel(), index);
	}

};