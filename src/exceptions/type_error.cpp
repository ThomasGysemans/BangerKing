#include "../../include/exceptions/type_error.hpp"

TypeError::TypeError(
  const Position& start,
  const Position& end,
  const string& d,
  const Context* ctx
): BaseRuntime(start, end, "Type Error", d, ctx) {}