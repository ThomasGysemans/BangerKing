#pragma once

#include "../position.hpp"
#include "../files.hpp"

class CustomError {
  protected:
    const Position pos_start;
    const Position pos_end;
    const std::string error_name;
    const std::string details;

  public:
    CustomError(
      const Position& start,
      const Position& end,
      const std::string& error,
      const std::string& d
    );

    std::string get_error_name() const;
    std::string get_details() const;
    std::string to_string() const;
};
