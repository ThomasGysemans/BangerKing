#pragma once

#include <any>
#include "../exceptions/undefined_behavior.hpp"
#include "../position.hpp"
#include "../types.hpp"
#include "../context.hpp"
using namespace std;

// Reference of "std::any"
// https://en.cppreference.com/w/cpp/utility/any

class Value {
  protected:
    const Type type;
    const Context* context = nullptr;
    Position pos_start;
    Position pos_end;
    any actual_value = nullptr;

    /// @brief Gets the actual value in C++.
    /// @return The reference to this value of type "any".
    const any* get_model() const;

  public:
    /// @brief Creates an instance of a computed value from the program.
    /// @param t The type of this value, from the `Type` enum.
    Value(Type t);

    virtual ~Value();

    // Making it a pure virtual method is important,
    // because we want the derived classes to call their own implementation
    // when calling it on a variable typed with 'Value*'.
    // It also makes the class "abstract",
    // meaning that creating an instance of "Value" isn't possible
    virtual Value* copy() const = 0;

    /// @brief Sets the position of this value in the program.
    /// @param pos_start The starting position of this value.
    /// @param pos_end The ending position of this value.
    void set_pos(const Position& start, const Position& end);

    /// @brief Sets the context in which this value was created
    /// @param ctx The context that created this value.
    void set_ctx(const Context* ctx);

    /// @brief Gets the name of the type associated with this value.
    /// @return The name of the type.
    Type get_type() const;

    /// @brief Gets the starting position of this value.
    /// @return The pointer to the starting position of this value.
    const Position* get_pos_start() const;

    /// @brief Gets the ending position of this value.
    /// @return The pointer to the ending position of this value.
    const Position* get_pos_end() const;

    /// @brief Indicates how this particular value can evaluate to true if used in a condition.
    /// @return a boolean
    virtual bool is_true() const { return false; }

    /// @brief Transforms the value into a string.
    /// @throw UndefinedBehaviorException if "to_string()" is called on an instance of "Value"
    /// @return The string representation of this value.
    virtual string to_string() const {
      if (!actual_value.has_value()) {
        return "null";
      }
      throw UndefinedBehaviorException("Cannot print a value of type '" + get_type_name(get_type()) + "'");
    }

    /// @brief We don't always want the same string representation when used in a concatenation or printed to the screen.
    /// @return The string to be printed when used on a `println()` .
    virtual string repr() const {
      return this->to_string();
    }
};