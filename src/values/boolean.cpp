#include "../../include/values/boolean.hpp"
#include "../../include/values/integer.hpp"
#include "../../include/values/string.hpp"
#include "../../include/values/double.hpp"
#include <cmath>
using namespace std;

BooleanValue::BooleanValue(bool v): Value(Type::BOOLEAN) {
  actual_value = any(v);
}

BooleanValue::BooleanValue(): Value(Type::BOOLEAN) {
  actual_value = BooleanValue::get_default_value();
}

bool BooleanValue::get_default_value() { return false; }

bool BooleanValue::get_actual_value() const { return any_cast<bool>(actual_value); }
bool BooleanValue::is_truthy() const { return get_actual_value(); }
string BooleanValue::to_string() const { return std::to_string(get_actual_value()); }
BooleanValue* BooleanValue::copy() const { return new BooleanValue(*this); }

unique_ptr<Value> BooleanValue::cast(Type output_type) const {
  unique_ptr<Value> cast_value;
  switch (output_type) {
    case Type::INT: cast_value = make_unique<IntegerValue>((int)get_actual_value()); break;
    case Type::DOUBLE: cast_value = make_unique<DoubleValue>((double)get_actual_value()); break;
    case Type::STRING: cast_value = make_unique<StringValue>(std::to_string(get_actual_value())); break;
    default:
      return nullptr;
  }
  cast_value->set_pos(pos_start, pos_end);
  cast_value->set_ctx(context);
  return cast_value;
}

// There is no operator override
// because the booleans are casted into integers
// by the interpreter, when used in a binary operation