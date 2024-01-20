#include "../include/runtime.hpp"

void RuntimeResult::reset() {
  value.reset();
  error.reset();
}

shared_ptr<BaseRuntimeError> RuntimeResult::get_error() const { return error; }
shared_ptr<Value> RuntimeResult::get_value() const { return value; }

shared_ptr<Value> RuntimeResult::read(unique_ptr<RuntimeResult> res) {
  if (res->get_error() != nullptr) error = res->get_error();
  return res->get_value();
}

void RuntimeResult::success(unique_ptr<Value> v) {
  reset();
  value = move(v); // transfers ownership of "v" to the shared_ptr "value"
} 

void RuntimeResult::failure(unique_ptr<BaseRuntimeError> err) {
  reset();
  error = move(err);
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