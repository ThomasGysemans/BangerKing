#pragma once

#include <string>
#include "../position.hpp"
using namespace std;

/// @brief Holds a generic custom node.
class CustomNode {
  // TODO: I don't think that making a copy of the positions
  // is necessary as it should not be changed at this point in the execution

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

    virtual string to_string() const = 0; // pure virtual method
};