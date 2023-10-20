#pragma once

#include "custom_node.hpp"
using namespace std;

class ModuloNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    ModuloNode(
      const CustomNode* a,
      const CustomNode* b
    );

    ~ModuloNode();

    const CustomNode* get_a() const;
    const CustomNode* get_b() const;
    string to_string() const override;
};