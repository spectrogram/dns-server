#pragma once
#include <iostream>
#include <exception>

class nxdomainException : public std::exception {
	virtual const char* what() const throw() {
		return "NXDOMAIN";
	}
};