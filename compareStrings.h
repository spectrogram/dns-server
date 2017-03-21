#pragma once
#include <iostream>
#include <algorithm>
#include <memory>

#include "edge.h"

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