#include "../../include/exceptions/exception.hpp"
using namespace std;

Exception::Exception(
  const string& t,
  const string& mess
): type(t), message(mess) {}

string Exception::get_type() const { return type; }
string Exception::to_string() const { return type + ": " + message; }