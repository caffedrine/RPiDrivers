//
// Created by curiosul on 1/14/19.
//

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>

struct Exception : public std::exception
{
	std::string ExceptionDescription;
	explicit Exception(std::string ss): ExceptionDescription(std::move(ss))
	{
	
	}
	
	Exception(const std::string &function, const std::string &ss): ExceptionDescription("[EXCEPTION] " +  function + ": " + ss )
	{
	
	}
	
	~Exception() noexcept override = default;
	
	const char* what() const noexcept override
	{
		return ExceptionDescription.c_str();
	}
};


#endif //RPIDRIVERS_EXCEPTION_H
