#pragma once

#include "custom_error.hpp"
using namespace std;

class IllegalCharError: public CustomError {
  public:
    IllegalCharError(
      const Position& start,
      const Position& end,
      const string& d
    );
};
