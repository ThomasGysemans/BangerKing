#pragma once

#include <list>
#include "value.hpp"
using namespace std;

class ListValue: public Value {
  list<shared_ptr<const Value>> elements;

  public:
    ListValue(list<shared_ptr<const Value>> elts);

    ~ListValue() = default;

    string to_string() const override;
    bool is_truthy() const override;
    ListValue* copy() const override;
    const list<shared_ptr<const Value>> get_elements() const;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the ListValue:
    /// - Type::INT => returns an integer with the length of the list.
    unique_ptr<Value> cast(Type output_type) const override;
};