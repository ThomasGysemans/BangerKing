#include "../../include/exceptions/type_overflow_error.hpp"

TypeOverflowError::TypeOverflowError(
  const Position& start,
  const Position& end,
  const string& d,
  shared_ptr<const Context> ctx
): BaseRuntimeError(start, end, "Type Overflow", d, ctx) {}