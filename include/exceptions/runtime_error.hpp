#pragma once

#include "base_runtime_error.hpp"

class RuntimeError final: public BaseRuntimeError {
  public:
    RuntimeError(
      const Position& start,
      const Position& end,
      const std::string& d,
      const std::shared_ptr<Context>& ctx
    );
};
