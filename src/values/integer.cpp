#include "../../include/values/integer.hpp"

IntegerValue::IntegerValue(int v): Value(Type::INT) {
  actual_value = any(v);
}

int IntegerValue::get_actual_value() const { return any_cast<int>(actual_value); }
bool IntegerValue::is_true() const { return get_actual_value() != 0; }
string IntegerValue::to_string() const { return std::to_string(get_actual_value()); }

IntegerValue* IntegerValue::operator+(const IntegerValue& other) const {
  return new IntegerValue{
    get_actual_value() + other.get_actual_value()
  };
}