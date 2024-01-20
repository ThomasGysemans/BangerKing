#pragma once

#include "base_runtime_error.hpp"
using namespace std;

class TypeError: public BaseRuntimeError {
  public:
    TypeError(
      const Position& start,
      const Position& end,
      const string& d,
      shared_ptr<const Context> ctx
    );
};
