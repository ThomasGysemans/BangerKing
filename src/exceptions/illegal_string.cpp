#include "../../include/exceptions/illegal_string_error.hpp"
using namespace std;

IllegalStringError::IllegalStringError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Illegal String", d) {}