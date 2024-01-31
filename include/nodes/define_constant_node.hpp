#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
#include "../types.hpp"

/// @brief Handles the creation of a variable.
/// It contains the initial value (if there is one, defaults to `nullptr`).
/// It stores the name of the variable as a string,
/// its type, and the name of the type (useful if it's a custom type).
class DefineConstantNode final: public CustomNode {
  const std::string var_name;
  std::unique_ptr<CustomNode> value_node;
  const Type type; // a constant must be of a native type

  public:
    DefineConstantNode(
      std::string var_name,
      std::unique_ptr<CustomNode> value,
      const Type& type,
      const Position& pos_start,
      const Position& pos_end
    );

    ~DefineConstantNode() override = default;

    /// @brief Gets the node holding the value of this new constant.
    /// @return The pointer to the node holding the value of this new constant.
    std::unique_ptr<CustomNode> retrieve_value_node();

    /// @brief Gets the name of the constant.
    /// @return The name of the constant.
    [[nodiscard]] std::string get_var_name() const;

    /// @brief Gets the native type of this constant from the `Type` enum.
    /// @return The native type of this constant.
    [[nodiscard]] Type get_type() const;

    [[nodiscard]] std::string to_string() const override;
};