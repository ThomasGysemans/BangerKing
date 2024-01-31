#pragma once

#include "custom_node.hpp"

// A node generated
// with the "!" token
// or the "not" keyword
class NotNode final: public CustomNode {
  std::unique_ptr<CustomNode> node;

  public:
    explicit NotNode(std::unique_ptr<CustomNode> n);

    ~NotNode() override = default;

    /// @brief Transfers the node it's negating
    std::unique_ptr<CustomNode> retrieve_node();

    [[nodiscard]] std::string to_string() const override;
};