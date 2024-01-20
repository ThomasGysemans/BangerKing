#pragma once

#include "custom_error.hpp"
#include "../context.hpp"

using namespace std;

class BaseRuntimeError: public CustomError {
  protected:
    shared_ptr<const Context> context;

  public:
    BaseRuntimeError(
      const Position& start,
      const Position& end,
      const string& name,
      const string& d,
      shared_ptr<const Context> ctx
    );
    
    string to_string() const;

  private:
    string generate_traceback() const;
};
