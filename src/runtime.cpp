#include "../include/runtime.hpp"

RuntimeResult::~RuntimeResult() {
  delete value;
  delete error;
}

void RuntimeResult::reset() {
  value = nullptr;
  error = nullptr;
}

CustomError* RuntimeResult::get_error() const { return error; }
Value* RuntimeResult::get_value() const { return value; }

Value* RuntimeResult::read(RuntimeResult* res) {
  error = res->get_error();
  return res->get_value();
}

RuntimeResult* RuntimeResult::success(Value* v) {
  reset();
  value = v;
  return this;
}

RuntimeResult* RuntimeResult::failure(CustomError* err) {
  reset();
  error = err;
  return this;
}

bool RuntimeResult::should_return() const {
  return error != nullptr;
}

string RuntimeResult::to_string() const {
  if (value == nullptr) {
    return "RuntimeResult()";
  }
  return "RuntimeResult(value = " + value->to_string() + ")";
}