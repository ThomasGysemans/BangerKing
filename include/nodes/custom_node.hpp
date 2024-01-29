#pragma once

#include "../position.hpp"

/// @brief Holds a generic custom node.
class CustomNode {
  const Position pos_start;
  const Position pos_end;

  public:
    const Position getStartingPosition() const;
    const Position getEndingPosition() const;

    CustomNode(
      const Position& start,
      const Position& end
    );

    virtual ~CustomNode() = default; // to allow the use of `dynamic_cast` (), it's "RTTI" (runtime type identification)

    virtual std::string to_string() const = 0; // pure virtual method
};