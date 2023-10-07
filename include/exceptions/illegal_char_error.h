#ifndef ILLEGAL_CHAR_ERROR_H
#define ILLEGAL_CHAR_ERROR_H

#include <iostream>
#include "custom_error.h"
using namespace std;

class IllegalCharError: public CustomError {
  public:
    IllegalCharError(
      const Position& start,
      const Position& end,
      const string& d
    ): CustomError(start, end, "Illegal Character", d) {}
};

#endif