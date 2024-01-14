#pragma once

#include <string>
using namespace std;

/// @brief Unfortunately, printing a double adds zeros at the end
/// To fix this, I delete all the zeros at the end manually,
/// and if the number was "3.0" then the dot is also removed.
/// It will print what looks like an integer,
/// but the value will still be a double.
/// https://stackoverflow.com/a/13709929/14522489
string double_to_string(double value);