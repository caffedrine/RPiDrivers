//
// Created by curiosul on 1/14/19.
//

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>

struct Exception : public std::exception
{
	std::string s;
	explicit Exception(std::string ss): s(std::move(ss)) {}
	Exception(std::string function, std::string ss) : s( function + "(): " + ss ) {}
	~Exception() throw () {}
	const char* what() const throw() override { return s.c_str(); }
};


#endif //RPIDRIVERS_EXCEPTION_H
