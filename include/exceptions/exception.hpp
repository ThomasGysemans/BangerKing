#pragma once

#include <string>
using namespace std;

class Exception {
  protected:
    string type;
    string message;

  public:
    Exception(const string& t, const string& mess);

    string get_type() const;
    string to_string() const;
};
