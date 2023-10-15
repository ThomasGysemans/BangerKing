#pragma once

#include "base_runtime_error.hpp"
using namespace std;

class RuntimeError: public BaseRuntime {
  public:
    RuntimeError(
      const Position& start,
      const Position& end,
      const string& d,
      const Context* ctx
    );
};
