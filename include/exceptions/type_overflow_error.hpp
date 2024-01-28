#pragma once

#include "base_runtime_error.hpp"
using namespace std;

/// @brief In case the program is trying to store a type it doesn't have the necessary memory for.
class TypeOverflowError: public BaseRuntimeError {
  public:
    TypeOverflowError(
      const Position& start,
      const Position& end,
      const string& d,
      shared_ptr<const Context> ctx
    );
};
