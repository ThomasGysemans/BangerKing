#pragma once

#include <any>
#include "../exceptions/undefined_behavior.hpp"
#include "../position.hpp"
#include "../types.hpp"
#include "../context.hpp"

// Reference of "std::any"
// https://en.cppreference.com/w/cpp/utility/any

class Value {
  protected:
    const Type type;
    std::shared_ptr<Context> context = nullptr; // TODO: is having the context in the Value that necessary?
    Position pos_start;
    Position pos_end;
    std::any actual_value = nullptr;

    /// @brief Gets the actual value in C++.
    /// @return The reference to this value of type "any".
    [[nodiscard]] const std::any* get_model() const;

  public:
    /// @brief Creates an instance of a computed value from the program.
    /// @param t The type of this value, from the `Type` enum.
    explicit Value(const Type& t);

    virtual ~Value() = default;

    // Making it a pure virtual method is important,
    // because we want the derived classes to call their own implementation
    // when calling it on a variable typed with 'Value*'.
    // It also makes the class "abstract",
    // meaning that creating an instance of "Value" isn't possible
    [[nodiscard]] virtual Value* copy() const = 0;

    /// @brief Sets the position of this value in the program.
    /// @param start The starting position of this value.
    /// @param end The ending position of this value.
    void set_pos(const Position& start, const Position& end);

    /// @brief Sets the context in which this value was created
    /// @param ctx The context that created this value.
    void set_ctx(const std::shared_ptr<Context>& ctx);

    /// @brief Gets the shared_ptr of the context in which this value was created.
    /// The context is not meant to be modified this way, so a const qualifier is applied to the context.
    /// @return The constant shared pointer to the context in which this value was created.
    std::shared_ptr<Context> get_ctx();

    /// @brief Gets the name of the type associated with this value.
    /// @return The name of the type.
    [[nodiscard]] Type get_type() const;

    /// @brief Gets the starting position of this value.
    /// @return The pointer to the starting position of this value.
    [[nodiscard]] const Position* get_pos_start() const;

    /// @brief Gets the ending position of this value.
    /// @return The pointer to the ending position of this value.
    [[nodiscard]] const Position* get_pos_end() const;

    /// @brief Indicates how this particular value can evaluate to true if used in a condition.
    /// @return `true` if the value can evaluate to `true` if used in a condition.
    [[nodiscard]] virtual bool is_truthy() const { return false; }

    /// @brief Transforms the value into a string.
    /// @throw UndefinedBehaviorException if "to_string()" is called on an instance of "Value"
    /// @return The string representation of this value.
    [[nodiscard]] virtual std::string to_string() const {
      if (!actual_value.has_value()) {
        return "null";
      }
      throw UndefinedBehaviorException("Cannot print a value of type '" + get_type_name(get_type()) + "'");
    }

    [[nodiscard]] virtual std::unique_ptr<Value> cast(Type output_type) const = 0;
};