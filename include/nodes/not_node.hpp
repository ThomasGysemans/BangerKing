#pragma once

#include "custom_node.hpp"
using namespace std;

// A node generated
// with the "!" token
// or the "not" keyword
class NotNode: public CustomNode {
  unique_ptr<CustomNode> node;

  public:
    NotNode(unique_ptr<CustomNode> n);

    ~NotNode() = default;

    /// @brief Transfers the node it's negating
    unique_ptr<CustomNode> retrieve_node();

    string to_string() const override;
};