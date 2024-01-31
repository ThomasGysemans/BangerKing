#include "../../include/exceptions/exception.hpp"
using namespace std;

Exception::Exception(
  string t,
  string mess
): type(move(t)), message(move(mess)) {}

string Exception::get_type() const { return type; }
string Exception::to_string() const { return type + ": " + message; }