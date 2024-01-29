#include "../../include/exceptions/arithmetic_error.hpp"
using namespace std;

ArithmeticError::ArithmeticError(
  const Position& start,
  const Position& end,
  const string& d,
  shared_ptr<const Context> ctx
): BaseRuntimeError(start, end, "Arithmetic Error", d, ctx) {}