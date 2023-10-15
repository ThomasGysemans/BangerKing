#pragma once

#include <list>
#include "value.hpp"
using namespace std;

class ListValue: public Value<> {
  list<const Value<>*> elements;

  public:
    ListValue(const list<const Value<>*>& elts);

    ~ListValue();

    string to_string() const override;
    bool is_true() const override;
    const list<const Value*>* get_elements() const;
};