#pragma once

#include "custom_error.hpp"
using namespace std;

class InvalidSyntaxError: public CustomError {
  public:
    InvalidSyntaxError(
      const Position& start,
      const Position& end,
      const string& d
    );
};
