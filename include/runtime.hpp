#pragma once

#include "values/value.hpp"
#include "exceptions/custom_error.hpp"
#include "context.hpp"
using namespace std;

/// @brief Keeps track of a runtime error, or if we should return/break/continue in a node.
class RuntimeResult {
  Value<>* value = nullptr;
  CustomError* error = nullptr;

  /// @brief Sets `value` and `error` to `nullptr`.
  void reset();

  public:
    CustomError* get_error() const;
    Value<>* get_value() const;

    /// @brief Registers an action during runtime and checks if an error has been thrown.
    /// @param res The previous action.
    /// @return The value passed to the original instance of RuntimeResult.
    Value<>* read(RuntimeResult* res);

    /// @brief Registers a successful action during runtime.
    /// @param v The value that has been successfully generated during runtime.
    /// @return The reference to `this`
    RuntimeResult* success(Value<> *v);

    /// @brief Registers an unsuccessful action during runtime.
    /// @param err The error that's just happened.
    /// @return The reference to `this`
    RuntimeResult* failure(CustomError* err);

    /// @brief Stops the program if there is an error, or if we should return, continue or break.
    /// @return A boolean.
    bool should_return() const;

    string to_string() const;
};
