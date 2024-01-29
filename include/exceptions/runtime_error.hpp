#pragma once

#include "base_runtime_error.hpp"

class RuntimeError: public BaseRuntimeError {
  public:
    RuntimeError(
      const Position& start,
      const Position& end,
      const std::string& d,
      std::shared_ptr<const Context> ctx
    );
};
