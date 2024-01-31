#include "../../include/values/value.hpp"
using namespace std;

const any* Value::get_model() const { return &actual_value; }

Value::Value(
  const Type& t
): type(t), pos_start(Position::getDefaultPos()), pos_end(Position::getDefaultPos()) {}

void Value::set_pos(const Position& start, const Position& end) {
  pos_start = start;
  pos_end = end;
}

void Value::set_ctx(const shared_ptr<Context>& ctx) { context = ctx; }
shared_ptr<Context> Value::get_ctx() { return context; }
Type Value::get_type() const { return type; }
const Position* Value::get_pos_start() const { return &pos_start; }
const Position* Value::get_pos_end() const { return &pos_end; }