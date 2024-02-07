#pragma once
#include "../custom_error.hpp"

class CompilerError final: public CustomError {
  public:
    CompilerError(
      Position start,
      Position end,
      std::string d
    );
};
