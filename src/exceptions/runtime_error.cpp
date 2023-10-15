#include "../../include/exceptions/runtime_error.hpp"

RuntimeError::RuntimeError(
  const Position& start,
  const Position& end,
  const string& d,
  const Context* ctx
): BaseRuntime(start, end, "Runtime Error", d, ctx) {}