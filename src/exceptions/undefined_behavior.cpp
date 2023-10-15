#include "../../include/exceptions/undefined_behavior.hpp"

UndefinedBehaviorException::UndefinedBehaviorException(
  const string& mess
): Exception("Undefined behavior", mess) {}