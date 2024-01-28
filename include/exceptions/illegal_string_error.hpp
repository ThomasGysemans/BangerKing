#pragma once

#include "custom_error.hpp"
using namespace std;

class IllegalStringError: public CustomError {
  public:
    IllegalStringError(
      const Position& start,
      const Position& end,
      const string& d
    );
};
