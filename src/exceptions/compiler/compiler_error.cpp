#include "../../../include/exceptions/compiler/compiler_error.hpp"
using namespace std;

CompilerError::CompilerError(
  Position start,
  Position end,
  std::string d
): CustomError(move(start), move(end), "Compiler Error", move(d)) {}