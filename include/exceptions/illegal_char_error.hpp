#pragma once

#include "custom_error.hpp"

class IllegalCharError final: public CustomError {
  public:
    IllegalCharError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
