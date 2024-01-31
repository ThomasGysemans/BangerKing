#pragma once

#include <list>
#include "value.hpp"

using list_of_values_ptr = std::list<std::shared_ptr<const Value>>;

class ListValue final: public Value {
  list_of_values_ptr elements;

  public:
    explicit ListValue(list_of_values_ptr elts);

    ~ListValue() override = default;

    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] bool is_truthy() const override;
    [[nodiscard]] ListValue* copy() const override;
    [[nodiscard]] list_of_values_ptr get_elements() const;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the ListValue:
    /// - Type::INT => returns an integer with the length of the list.
    [[nodiscard]] std::unique_ptr<Value> cast(Type output_type) const override;
};