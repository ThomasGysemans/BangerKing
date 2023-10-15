#pragma once

#include "value.hpp"

class IntegerValue: public Value<int> {
  public:
    IntegerValue(int v);

    int get_actual_value() const override;
    bool is_true() const override;

    IntegerValue operator+(IntegerValue& other) const;
};
