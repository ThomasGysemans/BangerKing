#pragma once

#include "custom_error.hpp"
#include "../context.hpp"

class BaseRuntimeError: public CustomError {
  protected:
    std::shared_ptr<Context> context;

  public:
    BaseRuntimeError(
      const Position& start,
      const Position& end,
      const std::string& name,
      const std::string& d,
      const std::shared_ptr<Context>& ctx
    );

    [[nodiscard]] std::string to_string() const override;

  private:
    [[nodiscard]] std::string generate_traceback() const;
};
