#include "../../include/exceptions/type_overflow_error.hpp"
using namespace std;

TypeOverflowError::TypeOverflowError(
  const Position& start,
  const Position& end,
  const string& d,
  const shared_ptr<Context>& ctx
): BaseRuntimeError(start, end, "Type Overflow", d, ctx) {}