#include "../../include/exceptions/runtime_error.hpp"
using namespace std;

RuntimeError::RuntimeError(
  const Position& start,
  const Position& end,
  const string& d,
  const shared_ptr<Context>& ctx
): BaseRuntimeError(start, end, "Runtime Error", d, ctx) {}