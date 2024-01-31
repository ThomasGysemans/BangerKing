#pragma once

#include "base_runtime_error.hpp"

/// @brief In case the program is trying to store a type it doesn't have the necessary memory for.
class TypeOverflowError final: public BaseRuntimeError {
  public:
    TypeOverflowError(
      const Position& start,
      const Position& end,
      const std::string& d,
      const std::shared_ptr<Context>& ctx
    );
};
