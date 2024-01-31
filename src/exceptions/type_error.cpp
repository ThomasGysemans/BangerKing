#include "../../include/exceptions/type_error.hpp"
using namespace std;

TypeError::TypeError(
  const Position& start,
  const Position& end,
  const string& d,
  const shared_ptr<Context>& ctx
): BaseRuntimeError(start, end, "Type Error", d, ctx) {}