#pragma once

#include "custom_error.hpp"

class IllegalStringError: public CustomError {
  public:
    IllegalStringError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
