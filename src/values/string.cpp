#include "../../include/values/string.hpp"
#include "../../include/values/integer.hpp"
using namespace std;

StringValue::StringValue(string v): Value(Type::STRING) {
  actual_value = any(v);
}

StringValue::StringValue(): Value(Type::STRING) {
  actual_value = StringValue::get_default_value();
}

string StringValue::get_default_value() { return ""; }
string StringValue::get_actual_value() const { return any_cast<string>(actual_value); }
bool StringValue::is_truthy() const { return get_actual_value().length() != 0; }
string StringValue::to_string() const { return get_actual_value(); }
StringValue* StringValue::copy() const { return new StringValue(*this); }

Value* StringValue::cast(Type output_type) const {
  Value* cast_value = nullptr;
  switch (output_type) {
    case Type::INT: cast_value = new IntegerValue(get_actual_value().length()); break;
    default:
      return nullptr;
  }
  cast_value->set_pos(pos_start, pos_end);
  cast_value->set_ctx(context);
  return cast_value;
} 

/*
*
* Concatenations
*
*/

StringValue* StringValue::operator+(const Value& other) const {
  return new StringValue{
    get_actual_value() + other.to_string()
  };
}

StringValue* StringValue::make_concatenation_rtl(const Value* left, const StringValue* right) {
  return new StringValue(
    left->to_string() + right->get_actual_value()
  );
}

/*
*
* Multiplications
*
*/

StringValue* StringValue::operator*(const IntegerValue& other) const {
  if (other.get_actual_value() < 0) return nullptr; // a RuntimeError will get thrown for invalid operation
  if (other.get_actual_value() == 0)  return new StringValue(); // empty string
  if (get_actual_value().length() == 1) {
    return new StringValue(
      string(other.get_actual_value(), get_actual_value().front())
    );
  } else {
    string new_str;
    new_str.reserve((unsigned int)get_actual_value().length() * other.get_actual_value());
    for (int i = 0; i < other.get_actual_value(); i++) {
      new_str.append(get_actual_value());
    }
    new_str.shrink_to_fit();
    return new StringValue(
      new_str
    );
  }
}
