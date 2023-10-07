#ifndef CUSTOM_ERROR_H
#define CUSTOM_ERROR_H

#include <iostream>
#include "../position.h"
using namespace std;

class CustomError {
  const string error_name;
  const string details;
  const Position pos_start;
  const Position pos_end;

  public:
    CustomError(
      const Position& start,
      const Position& end,
      const string& error,
      const string& d
    ): pos_start(start), pos_end(end), error_name(error), details(d) {}

    string to_string() const {
      string result = error_name + ": " + details + "\n";
      result += "File " + pos_start.get_filename() + ", line " + std::to_string(pos_start.get_ln() + 1) + "\n\n";
      result += "some arrows";
      return result;
    }
};

#endif