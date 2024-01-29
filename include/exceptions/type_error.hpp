#pragma once

#include "base_runtime_error.hpp"

class TypeError: public BaseRuntimeError {
  public:
    TypeError(
      const Position& start,
      const Position& end,
      const std::string& d,
      std::shared_ptr<const Context> ctx
    );
};
