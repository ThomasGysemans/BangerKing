#pragma once

#include <string>

class Exception {
  protected:
    std::string type;
    std::string message;

  public:
    Exception(const std::string& t, const std::string& mess);

    std::string get_type() const;
    std::string to_string() const;
};
