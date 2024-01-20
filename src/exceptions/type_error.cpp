#include "../../include/exceptions/type_error.hpp"

TypeError::TypeError(
  const Position& start,
  const Position& end,
  const string& d,
  shared_ptr<const Context> ctx
): BaseRuntimeError(start, end, "Type Error", d, ctx) {}