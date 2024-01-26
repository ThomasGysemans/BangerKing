#pragma once

#include "custom_error.hpp"
using namespace std;

class UnclosedStringError: public CustomError {
  public:
    UnclosedStringError(
      const Position& start,
      const Position& end,
      const string& d
    );
};
