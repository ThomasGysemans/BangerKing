#pragma once

#include "custom_error.hpp"
using namespace std;

class ExpectedCharError: public CustomError {
  public:
    ExpectedCharError(
      const Position& start,
      const Position& end,
      const string& d
    );
};
