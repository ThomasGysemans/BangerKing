#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <list>
#include "position.h"
#include "token.h"
using namespace std;

/// @brief Holds a generic custom node.
class CustomNode {
  // I don't think that making a copy of the positions
  // is necessary as it should not be changed at this point in the execution

  const Position* pos_start;
  const Position* pos_end;

  public:
    const Position& getStartingPosition() const { return *pos_start; }
    const Position& getEndingPosition() const { return *pos_end; }

    CustomNode(
      const Position& start,
      const Position& end
    ): pos_start(&start), pos_end(&end) {}

    virtual ~CustomNode() {} // to allow the use of `dynamic_cast` ()

    virtual string to_string() const {
      return "CustomNode()";
    }
};

// class EmptyCustomNode: public CustomNode {
//   public:
//     EmptyCustomNode(): CustomNode(Position::getDefaultPos().copy(), Position::getDefaultPos().copy()) {}
// };

class NumberNode: public CustomNode {
  const int value;

  public:
    NumberNode(
      const Token& token
    ): CustomNode(token.getStartingPosition(), token.getEndingPosition()), value(stoi(token.getStringValue())) {}

    const int getValue() const { return value; }

    string to_string() const override {
      return "NumberNode(" + std::to_string(value) + ")";
    }
};

class AddNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    AddNode(
      const CustomNode* a,
      const CustomNode* b
    ): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

    const CustomNode* get_a() const { return node_a; }
    const CustomNode* get_b() const { return node_b; }

    string to_string() const override {
      return "AddNode(" + node_a->to_string() + "+" + node_b->to_string() + ")";
    }
};

class SubstractNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    SubstractNode(
      const CustomNode* a,
      const CustomNode* b
    ): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

    const CustomNode* get_a() const { return node_a; }
    const CustomNode* get_b() const { return node_b; }

    string to_string() const override {
      return "SubstractNode(" + node_a->to_string() + "-" + node_b->to_string() + ")";
    }
};

class MultiplyNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    MultiplyNode(
      const CustomNode* a,
      const CustomNode* b
    ): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

    const CustomNode* get_a() const { return node_a; }
    const CustomNode* get_b() const { return node_b; }

    string to_string() const override {
      return "MultiplyNode(" + node_a->to_string() + "*" + node_b->to_string() + ")";
    }
};

class DivideNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    DivideNode(
      const CustomNode* a,
      const CustomNode* b
    ): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

    const CustomNode* get_a() const { return node_a; }
    const CustomNode* get_b() const { return node_b; }

    string to_string() const override {
      return "DivideNode(" + node_a->to_string() + "/" + node_b->to_string() + ")";
    }
};

class ModuloNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    ModuloNode(
      const CustomNode* a,
      const CustomNode* b
    ): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

    const CustomNode* get_a() const { return node_a; }
    const CustomNode* get_b() const { return node_b; }

    string to_string() const override {
      return "ModuloNode(" + node_a->to_string() + "%" + node_b->to_string() + ")";
    }
};

class PowerNode: public CustomNode {
  const CustomNode* node_a;
  const CustomNode* node_b;

  public:
    PowerNode(
      const CustomNode* a,
      const CustomNode* b
    ): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

    const CustomNode* get_a() const { return node_a; }
    const CustomNode* get_b() const { return node_b; }

    string to_string() const override {
      return "PowerNode(" + node_a->to_string() + "**" + node_b->to_string() + ")";
    }
};

// Unary operations
// like -5 or +5

class PlusNode: public CustomNode {
  const CustomNode* node;

  public:
    PlusNode(
      const CustomNode* n
    ): CustomNode((*n).getStartingPosition(), (*n).getEndingPosition()), node(n) {}

    const CustomNode* get_node() const { return node; }

    string to_string() const override {
      return "(+" + node->to_string() + ")";
    }
};

class MinusNode: public CustomNode {
  const CustomNode* node;

  public:
    MinusNode(
      const CustomNode* n
    ): CustomNode((*n).getStartingPosition(), (*n).getEndingPosition()), node(n) {}

    const CustomNode* get_node() const { return node; }

    string to_string() const override {
      return "(-" + node->to_string() + ")";
    }
};

/// @brief A list of nodes. It can also contain the whole program, as it is just a list of nodes too.
class ListNode: public CustomNode {
  // A copy of the statements produced by the Parser
  // is necessary as the reference to the list created inside the parser
  // is lost when at the end of the process. 
  list<const CustomNode*> element_nodes;

  public:
    ListNode(
      list<const CustomNode*> nodes,
      const Position& start,
      const Position& end
    ): CustomNode(start, end), element_nodes(nodes) {}

    // Note that "nodes.front()" is of type `const CustomNode*`
    // so `->` is necessary to access `getStartingPosition()`
    ListNode(
      list<const CustomNode*> nodes
    ): CustomNode(nodes.front()->getStartingPosition(), nodes.back()->getEndingPosition()), element_nodes(nodes) {}

    ~ListNode() {
      for (auto it = element_nodes.begin(); it != element_nodes.end(); ++it) {
        delete *it;
      }
    }

    list<const CustomNode*> get_element_nodes() const { return element_nodes; }

    string to_string() const override {
      list<const CustomNode*>::const_iterator iter = element_nodes.begin();
      string result = "[" + (*iter)->to_string(); // (or (**iter).to_string() it's the same thing
      ++iter;
      while (iter != element_nodes.end()) {
        result += ", " + (*iter)->to_string();
        ++iter;
      }
      return result + "]";
    }
};

#endif