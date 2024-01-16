#pragma once

#include <list>
#include "value.hpp"
using namespace std;

class ListValue: public Value {
  list<const Value*> elements;

  public:
    ListValue(const list<const Value*>& elts);

    ~ListValue();

    string to_string() const override;
    bool is_truthy() const override;
    ListValue* copy() const override;
    const list<const Value*>* get_elements() const;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the ListValue:
    /// - Type::INT => returns an integer with the length of the list.
    Value* cast(Type output_type) const override;
};