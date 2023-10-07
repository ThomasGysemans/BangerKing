#ifndef EXPECTED_CHAR_ERROR_H
#define EXPECTED_CHAR_ERROR_H

#include <iostream>
#include "custom_error.h"
using namespace std;

class ExpectedCharError: public CustomError {
  public:
    ExpectedCharError(
      const Position& start,
      const Position& end,
      const string& d
    ): CustomError(start, end, "Expected Character", d) {}
};

#endif