#include "../../include/values/integer.hpp"
#include "../../include/values/double.hpp"
#include <cmath>
using namespace std;

IntegerValue::IntegerValue(int v): Value(Type::INT) {
  actual_value = any(v);
}

IntegerValue::IntegerValue(): Value(Type::INT) {
  actual_value = IntegerValue::get_default_value();
}

int IntegerValue::get_default_value() { return 0; }

int IntegerValue::get_actual_value() const { return any_cast<int>(actual_value); }
bool IntegerValue::is_truthy() const { return get_actual_value() != 0; }
string IntegerValue::to_string() const { return std::to_string(get_actual_value()); }
IntegerValue* IntegerValue::copy() const { return new IntegerValue(*this); }

unique_ptr<Value> IntegerValue::cast(Type output_type) const {
  unique_ptr<Value> cast_value;
  switch (output_type) {
    case Type::DOUBLE: cast_value = make_unique<DoubleValue>((double)get_actual_value()); break;
    default:
      return nullptr;
  }
  cast_value->set_pos(pos_start, pos_end);
  cast_value->set_ctx(context);
  return cast_value;
}

/*
*
* Additions
*
*/

IntegerValue* IntegerValue::operator+(const IntegerValue& other) const {
  return new IntegerValue{
    get_actual_value() + other.get_actual_value()
  };
}

DoubleValue* IntegerValue::operator+(const DoubleValue& other) const {
  return new DoubleValue{
    get_actual_value() + other.get_actual_value()
  };
}

/*
*
* Substractions
*
*/

IntegerValue* IntegerValue::operator-(const IntegerValue& other) const {
  return new IntegerValue(
    get_actual_value() - other.get_actual_value()
  );
}

DoubleValue* IntegerValue::operator-(const DoubleValue& other) const {
  return new DoubleValue{
    get_actual_value() - other.get_actual_value()
  };
}

/*
*
* Multiplications
*
*/

IntegerValue* IntegerValue::operator*(const IntegerValue& other) const {
  return new IntegerValue(
    get_actual_value() * other.get_actual_value()
  );
}

DoubleValue* IntegerValue::operator*(const DoubleValue& other) const {
  return new DoubleValue{
    get_actual_value() * other.get_actual_value()
  };
}

/*
*
* Divisions
*
*/

IntegerValue* IntegerValue::operator/(const IntegerValue& other) const {
  return new IntegerValue(
    get_actual_value() / other.get_actual_value()
  );
}

DoubleValue* IntegerValue::operator/(const DoubleValue& other) const {
  return new DoubleValue{
    get_actual_value() / other.get_actual_value()
  };
}

/*
*
* Modulos
*
*/

IntegerValue* IntegerValue::operator%(const IntegerValue& other) const {
  return new IntegerValue(
    get_actual_value() % other.get_actual_value()
  );
}

DoubleValue* IntegerValue::operator%(const DoubleValue& other) const {
  return new DoubleValue{
    std::fmod(get_actual_value(), other.get_actual_value())
  };
}