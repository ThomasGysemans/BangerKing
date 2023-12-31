#pragma once

#include "custom_node.hpp"
using namespace std;

class SubstractNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    SubstractNode(
      const CustomNode* a,
      const CustomNode* b
    );

    ~SubstractNode();

    const CustomNode* get_a() const;
    const CustomNode* get_b() const;
    string to_string() const override;
};