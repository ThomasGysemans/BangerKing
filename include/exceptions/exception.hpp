#pragma once

#include <string>

class Exception: public std::exception {
  protected:
    std::string type;
    std::string message;

  public:
    Exception(std::string t, std::string mess);

    [[nodiscard]] std::string get_type() const;
    [[nodiscard]] std::string to_string() const;
};
