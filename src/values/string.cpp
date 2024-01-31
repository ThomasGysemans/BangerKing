#include "../../include/values/string.hpp"
#include "../../include/values/integer.hpp"
using namespace std;

StringValue::StringValue(const string& v): Value(STRING) {
  actual_value = any(v);
}

StringValue::StringValue(): Value(STRING) {
  actual_value = get_default_value();
}

string StringValue::get_default_value() { return ""; }
string StringValue::get_actual_value() const { return any_cast<string>(actual_value); }
bool StringValue::is_truthy() const { return !get_actual_value().empty(); }
string StringValue::to_string() const { return get_actual_value(); }
StringValue* StringValue::copy() const { return new StringValue(*this); }

unique_ptr<Value> StringValue::cast(const Type output_type) const {
  unique_ptr<Value> cast_value = nullptr;
  switch (output_type) {
    case INT: cast_value = make_unique<IntegerValue>(get_actual_value().length()); break;
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
    new_str.reserve(static_cast<unsigned int>(get_actual_value().length() * other.get_actual_value()));
    for (int i = 0; i < other.get_actual_value(); i++) {
      new_str.append(get_actual_value());
    }
    new_str.shrink_to_fit();
    return new StringValue(
      new_str
    );
  }
}
