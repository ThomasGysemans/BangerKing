#include "../../include/exceptions/undefined_behavior.hpp"
using namespace std;

UndefinedBehaviorException::UndefinedBehaviorException(
  const string& mess
): Exception("Undefined behavior", mess) {}