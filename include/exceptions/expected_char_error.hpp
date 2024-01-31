#pragma once

#include "custom_error.hpp"

class ExpectedCharError final: public CustomError {
  public:
    ExpectedCharError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
