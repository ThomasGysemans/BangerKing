#pragma once

#include "../position.hpp"
#include "../files.hpp"
using namespace std;

class CustomError {
  protected:
    const Position pos_start;
    const Position pos_end;
    const string error_name;
    const string details;

  public:
    CustomError(
      const Position& start,
      const Position& end,
      const string& error,
      const string& d
    );

    string to_string() const;
};
