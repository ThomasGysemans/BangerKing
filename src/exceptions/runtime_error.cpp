#include "../../include/exceptions/runtime_error.hpp"

RuntimeError::RuntimeError(
  const Position& start,
  const Position& end,
  const string& d,
  shared_ptr<const Context> ctx
): BaseRuntimeError(start, end, "Runtime Error", d, ctx) {}