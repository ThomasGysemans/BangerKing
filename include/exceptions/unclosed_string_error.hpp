#pragma once

#include "custom_error.hpp"

class UnclosedStringError: public CustomError {
  public:
    UnclosedStringError(
      const Position& start,
      const Position& end,
      const std::string& d
    );
};
