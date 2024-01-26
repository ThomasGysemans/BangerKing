#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class BooleanNode: public CustomNode {
  const Token token;

  public:
    BooleanNode(const Token token);

    ~BooleanNode() = default;

    bool is_true() const;
    const Token* getToken() const;
    string to_string() const override;
};