#include "../../include/values/value.hpp"
#include "../../include/values/integer.hpp"
#include "../../include/values/double.hpp"
#include "../../include/values/list.hpp"
using namespace std;

ListValue::ListValue(
  const list<const Value*>& elts
): Value(Type::LIST), elements(elts) {}

ListValue::~ListValue() {
  for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
    delete *iter;
  }
  elements.clear();
}

const list<const Value*>* ListValue::get_elements() const { return &elements; }
bool ListValue::is_truthy() const { return !elements.empty(); }
ListValue* ListValue::copy() const { return new ListValue(*this); }

string ListValue::to_string() const {
  list<const Value*>::const_iterator iter = elements.begin();
  string res = "[" + (*iter)->to_string();
  ++iter;
  while (iter != elements.end()) {
    res += ", " + (*iter)->to_string();
  }
  return res + "]";
}

Value* ListValue::cast(Type output_type) const {
  Value* cast_value = nullptr;
  switch (output_type) {
    case Type::INT: cast_value = new IntegerValue(elements.size()); break;
    default:
      return nullptr;
  }
  cast_value->set_pos(pos_start, pos_end);
  cast_value->set_ctx(context);
  return cast_value;
}