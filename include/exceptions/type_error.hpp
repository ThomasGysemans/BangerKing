#pragma once

#include "base_runtime_error.hpp"

class TypeError final: public BaseRuntimeError {
  public:
    TypeError(
      const Position& start,
      const Position& end,
      const std::string& d,
      const std::shared_ptr<Context>& ctx
    );
};
