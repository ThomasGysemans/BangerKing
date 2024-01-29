#pragma once

#include "custom_error.hpp"

class ExpectedCharError: public CustomError {
  public:
    ExpectedCharError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
