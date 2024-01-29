#include "../../include/exceptions/custom_error.hpp"
#include "../../include/utils/string_with_arrows.hpp"
using namespace std;

CustomError::CustomError(
  const Position& start,
  const Position& end,
  const string& error,
  const string& d
): pos_start(start), pos_end(end), error_name(error), details(d) {}

string CustomError::to_string() const {
  string result = error_name + ": " + details + "\n";
  const string text = *(READ_FILES.at(pos_start.get_filename()));
  result += "File " + pos_start.get_filename() + ", line " + std::to_string(pos_start.get_ln() + 1) + "\n\n";
  result += string_with_arrows(text, pos_start, pos_end);
  return result;
}

string CustomError::get_error_name() const { return error_name; }
string CustomError::get_details() const { return details; }