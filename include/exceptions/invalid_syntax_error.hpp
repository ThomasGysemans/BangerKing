#pragma once

#include "custom_error.hpp"

class InvalidSyntaxError final: public CustomError {
  public:
    InvalidSyntaxError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
