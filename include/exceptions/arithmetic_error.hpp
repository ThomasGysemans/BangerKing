#pragma once

#include "base_runtime_error.hpp"

class ArithmeticError final: public BaseRuntimeError {
  public:
    ArithmeticError(
      const Position& start,
      const Position& end,
      const std::string& d,
      const std::shared_ptr<Context>& ctx
    );
};
