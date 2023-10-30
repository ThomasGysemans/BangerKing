#pragma once

#include "base_runtime_error.hpp"
using namespace std;

class TypeError: public BaseRuntime {
  public:
    TypeError(
      const Position& start,
      const Position& end,
      const string& d,
      const Context* ctx
    );
};
