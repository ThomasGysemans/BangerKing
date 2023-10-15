#pragma once

#include <any>
#include "../exceptions/undefined_behavior.hpp"
#include "../position.hpp"
#include "../types.hpp"
#include "../context.hpp"
using namespace std;

// ? NOTE: templated classes MUST be defined in their header file, or a "undefined symbol" error will be triggered at compile time

// Reference of "std::any"
// https://en.cppreference.com/w/cpp/utility/any

template <typename T = any>
class Value {
  protected:
    const Type type;
    const Context* context = nullptr;
    Position pos_start;
    Position pos_end;
    any actual_value = nullptr;

  public:
    /// @brief Creates an instance of a computed value from the program.
    /// @param t The type of this value, from the `Type` enum.
    Value(Type t): type(t), pos_start(Position::getDefaultPos()), pos_end(Position::getDefaultPos()) {}

    virtual ~Value() {}

    /// @brief Sets the position of this value in the program.
    /// @param pos_start The starting position of this value.
    /// @param pos_end The ending position of this value.
    void set_pos(const Position& start, const Position& end) {
      pos_start = start;
      pos_end = end;
    }

    /// @brief Sets the context in which this value was created
    /// @param ctx The context that created this value.
    void set_ctx(const Context* ctx) {
      context = ctx;
    }

    string get_type() const { return get_type_name(type); }

    /// @brief Gets the actual value in C++.
    /// @return The actual value, or it throws an error of type `UndefinedBehaviorException` if the value didn't override this method.
    virtual T get_actual_value() const {
      throw UndefinedBehaviorException("Undefined type of value.");
    }

    /// @brief Indicates how this particular value can evaluate to true if used in a condition.
    /// @return a boolean
    virtual bool is_true() const { return false; }

    /// @brief Transforms the value into a string.
    /// @return The string representation of this value.
    virtual string to_string() const {
      if (!actual_value.has_value()) {
        return "null";
      }
      throw UndefinedBehaviorException("Cannot print a value of type '" + get_type() + "'");
    }

    /// @brief We don't always want the same string representation when used in a concatenation or printed to the screen.
    /// @return The string to be printed when used on a `println()` .
    virtual string repr() const {
      return this->to_string();
    }

    /// @brief Creates a deep copy of this instance.
    /// @return A deep copy of this instance.
    Value<T> copy() const {
      return Value(*this);
    }
};