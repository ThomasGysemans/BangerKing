#ifndef INVALID_SYNTAX_ERROR_H
#define INVALID_SYNTAX_ERROR_H

#include <iostream>
#include "custom_error.h"
using namespace std;

class InvalidSyntaxError: public CustomError {
  public:
    InvalidSyntaxError(
      const Position& start,
      const Position& end,
      const string& d
    ): CustomError(start, end, "Invalid Syntax", d) {}
};

#endif