#pragma once

#include "exception.hpp"

class UndefinedBehaviorException final: public Exception {
  public:
    // Basically, the explicit keyword tells the compiler that this is not possible:
    // `UndefinedBehaviorException e = "yoyo"` (which is possible without the "explicit" keyword)
    // since this is a constructor with a single non-default parameter.
    // No copy is made during initialization.
    explicit UndefinedBehaviorException(
      const std::string& mess
    );
};
