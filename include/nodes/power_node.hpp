#pragma once

#include "custom_node.hpp"
using namespace std;

class PowerNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    PowerNode(
      const CustomNode* a,
      const CustomNode* b
    );

    const CustomNode* get_a() const;
    const CustomNode* get_b() const;
    string to_string() const override;
};