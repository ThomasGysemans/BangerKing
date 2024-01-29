#pragma once

#include "exception.hpp"

class UndefinedBehaviorException: public Exception {
  public:
    UndefinedBehaviorException(
      const std::string& mess
    );
};
