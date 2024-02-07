#pragma once

#include "../position.hpp"
#include "node_type.hpp"

/// @brief Holds a generic custom node.
class CustomNode {
  const Position pos_start;
  const Position pos_end;
  const NodeType::Type type;

  public:
    [[nodiscard]] Position getStartingPosition() const;
    [[nodiscard]] Position getEndingPosition() const;

    CustomNode(
      Position start,
      Position end,
      const NodeType::Type& type
    );

    [[nodiscard]] NodeType::Type getNodeType() const;

    virtual ~CustomNode() = default; // to allow the use of `dynamic_cast` (), it's "RTTI" (runtime type identification)

    [[nodiscard]] virtual std::string to_string() const = 0; // pure virtual method

    /// @brief The representation of a literal,
    /// which is not necessarily the same as `to_string()`.
    /// Not all nodes are literals.
    [[nodiscard]] virtual std::string literal() const {
      return to_string();
    }
};