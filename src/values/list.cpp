#include "../../include/values/value.hpp"
#include "../../include/values/integer.hpp"
#include "../../include/values/list.hpp"
using namespace std;

ListValue::ListValue(
  list<shared_ptr<const Value>> elts
): Value(LIST), elements(move(elts)) {}

list<shared_ptr<const Value>> ListValue::get_elements() const { return elements; }
bool ListValue::is_truthy() const { return !elements.empty(); }
ListValue* ListValue::copy() const { return new ListValue(*this); }

string ListValue::to_string() const {
  if (elements.empty()) {
    return "[]";
  }
  auto iter = elements.begin();
  string res = "[" + (*iter)->to_string();
  ++iter;
  while (iter != elements.end()) {
    res += ", " + (*iter)->to_string();
  }
  return res + "]";
}

unique_ptr<Value> ListValue::cast(const Type output_type) const {
  unique_ptr<Value> cast_value = nullptr;
  switch (output_type) {
    case INT: cast_value = make_unique<IntegerValue>(elements.size()); break;
    default:
      return nullptr;
  }
  cast_value->set_pos(pos_start, pos_end);
  cast_value->set_ctx(context);
  return cast_value;
}