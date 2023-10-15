#pragma once

#include "custom_error.hpp"
#include "../context.hpp"

using namespace std;

class BaseRuntime: public CustomError {
  protected:
    const Context* context;

  public:
    BaseRuntime(
      const Position& start,
      const Position& end,
      const string& name,
      const string& d,
      const Context* ctx = nullptr
    );
    
    string to_string() const;

  private:
    string generate_traceback() const;
};
