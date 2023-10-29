#include "../../include/values/value.hpp"
#include "../../include/values/list.hpp"

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
bool ListValue::is_true() const { return !elements.empty(); }
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