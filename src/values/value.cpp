#include "../../include/values/value.hpp"

const any* Value::get_model() const { return &actual_value; }

Value::Value(
  Type t
): type(t), pos_start(Position::getDefaultPos()), pos_end(Position::getDefaultPos()) {}

Value::~Value() { } // TODO: not sure that implementing it is necessary

void Value::set_pos(const Position& start, const Position& end) {
  pos_start = start;
  pos_end = end;
}

void Value::set_ctx(const Context* ctx) { context = ctx; }
string Value::get_type() const { return get_type_name(type); }
const Position* Value::get_pos_start() const { return &pos_start; }
const Position* Value::get_pos_end() const { return &pos_end; }

Value* Value::copy() const {
  return new Value(*this);
}