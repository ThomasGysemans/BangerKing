#pragma once

#include "custom_error.hpp"
#include "../context.hpp"

class BaseRuntimeError: public CustomError {
  protected:
    std::shared_ptr<const Context> context;

  public:
    BaseRuntimeError(
      const Position& start,
      const Position& end,
      const std::string& name,
      const std::string& d,
      std::shared_ptr<const Context> ctx
    );
    
    std::string to_string() const;

  private:
    std::string generate_traceback() const;
};
