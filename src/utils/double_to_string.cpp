#include "../../include/utils/double_to_string.hpp"
using namespace std;

string double_to_string(double value) {
  string str = to_string(value);
  str.erase ( str.find_last_not_of('0') + 1, string::npos );
  str.erase ( str.find_last_not_of('.') + 1, string::npos );
  return str;
}
