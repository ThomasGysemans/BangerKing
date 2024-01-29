#pragma once

#include "custom_error.hpp"

class IllegalCharError: public CustomError {
  public:
    IllegalCharError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
