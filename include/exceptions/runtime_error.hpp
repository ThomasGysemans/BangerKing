#pragma once

#include "base_runtime_error.hpp"
using namespace std;

class RuntimeError: public BaseRuntimeError {
  public:
    RuntimeError(
      const Position& start,
      const Position& end,
      const string& d,
      shared_ptr<const Context> ctx
    );
};
