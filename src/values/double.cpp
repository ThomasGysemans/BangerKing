#include "../../include/values/double.hpp"
#include "../../include/values/integer.hpp"
#include "../../include/utils/double_to_string.hpp"
#include <cmath>
using namespace std;

DoubleValue::DoubleValue(double v): Value(DOUBLE) {
  actual_value = any(v);
}

DoubleValue::DoubleValue(): Value(DOUBLE) {
  actual_value = get_default_value();
}

double DoubleValue::get_default_value() { return 0.0; }
double DoubleValue::get_actual_value() const { return any_cast<double>(actual_value); }
bool DoubleValue::is_truthy() const { return get_actual_value() != 0.0; }
string DoubleValue::to_string() const { return double_to_string(get_actual_value()); }
DoubleValue* DoubleValue::copy() const { return new DoubleValue(*this); }

unique_ptr<Value> DoubleValue::cast(Type output_type) const {
  unique_ptr<Value> cast_value;
  switch (output_type) {
    case INT: cast_value = make_unique<IntegerValue>(static_cast<int>(get_actual_value())); break;
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

DoubleValue* DoubleValue::operator+(const DoubleValue& other) const {
  return new DoubleValue{
    get_actual_value() + other.get_actual_value()
  };
}

DoubleValue* DoubleValue::operator+(const IntegerValue& other) const {
  return new DoubleValue{
    get_actual_value() + other.get_actual_value()
  };
}

/*
*
* Substractions
*
*/

DoubleValue* DoubleValue::operator-(const DoubleValue& other) const {
  return new DoubleValue(
    get_actual_value() - other.get_actual_value()
  );
}

DoubleValue* DoubleValue::operator-(const IntegerValue& other) const {
  return new DoubleValue(
    get_actual_value() - other.get_actual_value()
  );
}

/*
*
* Multiplications
*
*/

DoubleValue* DoubleValue::operator*(const DoubleValue& other) const {
  return new DoubleValue(
    get_actual_value() * other.get_actual_value()
  );
}

DoubleValue* DoubleValue::operator*(const IntegerValue& other) const {
  return new DoubleValue(
    get_actual_value() * other.get_actual_value()
  );
}

/*
*
* Divisions
*
*/

DoubleValue* DoubleValue::operator/(const DoubleValue& other) const {
  if (other.get_actual_value() == 0.0) {
    return nullptr;
  }
  return new DoubleValue(
    get_actual_value() / other.get_actual_value()
  );
}

DoubleValue* DoubleValue::operator/(const IntegerValue& other) const {
  if (other.get_actual_value() == 0) {
    return nullptr;
  }
  return new DoubleValue(
    get_actual_value() / other.get_actual_value()
  );
}

/*
*
* Modulos
*
*/

DoubleValue* DoubleValue::operator%(const DoubleValue& other) const {
  if (other.get_actual_value() == 0.0) {
    return nullptr;
  }
  return new DoubleValue(
    std::fmod(get_actual_value(), other.get_actual_value())
  );
}

DoubleValue* DoubleValue::operator%(const IntegerValue& other) const {
  if (other.get_actual_value() == 0) {
    return nullptr;
  }
  return new DoubleValue(
    std::fmod(get_actual_value(), other.get_actual_value())
  );
}