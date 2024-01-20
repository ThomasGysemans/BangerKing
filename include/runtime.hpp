#pragma once

#include "values/value.hpp"
#include "exceptions/base_runtime_error.hpp"
#include "context.hpp"
using namespace std;

/// @brief Keeps track of a runtime error, or if we should return/break/continue in a node.
class RuntimeResult {
  // Here, I'm using shared pointers
  // because I want to be able to share the pointers
  // in multiple scopes, while still having them here.
  shared_ptr<Value> value;
  shared_ptr<BaseRuntimeError> error;

  /// @brief Sets `value` and `error` to `nullptr`.
  void reset();

  public:
    /// @brief Returns a copy of the pointer to the BaseRuntimeError it might hold.
    /// @return The shared pointer to the error, or `nullptr` if there is none.
    shared_ptr<BaseRuntimeError> get_error() const;

    /// @brief Returns a copy of the pointer to the Value it might hold.
    /// @return The shared pointer to the value, or `nullptr` if there is none.
    shared_ptr<Value> get_value() const;

    // The default logic of a destructor is enough
    // (I don't need to manually deallocate the members).
    ~RuntimeResult() = default;

    /// @brief Registers an action during runtime and checks if an error has been thrown.
    /// The given RuntimeResult (`res`) will get deallocated because it is transferred to this function.
    /// @param res The previous action.
    /// @return The value passed to the original instance of RuntimeResult.
    shared_ptr<Value> read(unique_ptr<RuntimeResult> res);

    /// @brief Registers a successful action during runtime.
    /// The ownership of `v` is transferred to the `value` member of this class.
    /// @param v The value that has been successfully generated during runtime.
    /// @return The reference to `this`
    RuntimeResult* success(unique_ptr<Value> v);

    /// @brief Registers an unsuccessful action during runtime.
    /// The ownership of `err` is transferred to the `error` member of this class.
    /// @param err The error that's just happened.
    /// @return The reference to `this`
    RuntimeResult* failure(unique_ptr<BaseRuntimeError> err);

    /// @brief Stops the program if there is an error, or if we should return, continue or break.
    /// @return A boolean.
    bool should_return() const;

    string to_string() const;
};
