#pragma once

#include "exception.hpp"
using namespace std;

class UndefinedBehaviorException: public Exception {
  public:
    UndefinedBehaviorException(
      const string& mess
    );
};
