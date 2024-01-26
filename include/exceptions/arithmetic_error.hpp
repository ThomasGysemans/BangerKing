#pragma once

#include "base_runtime_error.hpp"
using namespace std;

class ArithmeticError: public BaseRuntimeError {
  public:
    ArithmeticError(
      const Position& start,
      const Position& end,
      const string& d,
      shared_ptr<const Context> ctx
    );
};
