#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
#include "../types.hpp"

/// @brief Handles the creation of a variable.
/// It contains the initial value (if there is one, defaults to `nullptr`).
/// It stores the name of the variable as a string,
/// its type, and the name of the type (useful if it's a custom type).
class VarAssignmentNode final: public CustomNode {
  const std::string var_name;
  std::unique_ptr<CustomNode> value_node; // can be "nullptr" if the variable doesn't have an initial value
  const std::string type_name; // in case the type is the instance of a custom object

  public:
    VarAssignmentNode(
      std::string var_name,
      std::unique_ptr<CustomNode> value,
      const Token& type_tok,
      const Position& pos_start,
      const Position& pos_end
    );

    ~VarAssignmentNode() override = default;

    /// @brief Gets the node holding the initial value of this new variable.
    /// @return The pointer to the node holding the initial value of this new variable.
    std::unique_ptr<CustomNode> retrieve_value_node();

    /// @brief Gets the name of the variable.
    /// @return The name of the variable.
    [[nodiscard]] std::string get_var_name() const;

    /// @brief Gets the exact type of this variable.
    [[nodiscard]] Type get_type() const;

    /// @brief Whether or not this variable assignment has an initial value.
    /// @return `true` if this new variable has an initial value.
    [[nodiscard]] bool has_value() const;

    /// @brief Gets the name of the type of this new variable.
    /// @return The type name of this variable.
    [[nodiscard]] std::string get_type_name() const;

    [[nodiscard]] std::string to_string() const override;
};