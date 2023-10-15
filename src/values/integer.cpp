#include "../../include/values/integer.hpp"

IntegerValue::IntegerValue(int v): Value(Type::INT) {
  actual_value = any_cast<int>(v);
}

int IntegerValue::get_actual_value() const { return any_cast<int>(actual_value); }
bool IntegerValue::is_true() const { return get_actual_value() != 0; }

IntegerValue IntegerValue::operator+(IntegerValue& other) const {
  return IntegerValue{
    get_actual_value() + other.get_actual_value()
  };
}