#pragma once

#include <exception>

class DataException : public std::exception
{
public:
    virtual const char *what() const throw() { return "Issue loading data."; }
};