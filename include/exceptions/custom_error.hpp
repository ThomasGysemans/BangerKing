#pragma once

#include "../position.hpp"
#include "../files.hpp"

class CustomError: public std::exception {
  protected:
    const Position pos_start;
    const Position pos_end;
    const std::string error_name;
    const std::string details;

  public:
    CustomError(
      Position start,
      Position end,
      std::string error,
      std::string d
    );

    [[nodiscard]] std::string get_error_name() const;
    [[nodiscard]] std::string get_details() const;
    [[nodiscard]] virtual std::string to_string() const;
};
