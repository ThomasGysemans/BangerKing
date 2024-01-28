#include "../include/interpreter.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/symbol_table.hpp"
#include "../include/exceptions/undefined_behavior.hpp"
#include "../include/exceptions/runtime_error.hpp"
#include "../include/exceptions/type_error.hpp"
#include "../include/exceptions/type_overflow_error.hpp"

// Since `shared_ctx` is static,
// it must be redeclared here so that the compiler knows it exists.
shared_ptr<Context> Interpreter::shared_ctx = nullptr;

void Interpreter::set_shared_ctx(shared_ptr<Context> ctx) {
  shared_ctx = ctx;
}

unique_ptr<RuntimeResult> Interpreter::visit(unique_ptr<CustomNode> node) {
  if (shared_ctx == nullptr) {
    throw string("A context was not provided for interpretation.");
  }
  if (instanceof<ListNode>(node.get())) {
    return visit_ListNode(cast_node<ListNode>(move(node)));
  } else if (instanceof<IntegerNode>(node.get())) {
    return visit_IntegerNode(cast_node<IntegerNode>(move(node)));
  } else if (instanceof<DoubleNode>(node.get())) {
    return visit_DoubleNode(cast_node<DoubleNode>(move(node)));
  } else if (instanceof<MinusNode>(node.get())) {
    return visit_MinusNode(cast_node<MinusNode>(move(node)));
  } else if (instanceof<PlusNode>(node.get())) {
    return visit_PlusNode(cast_node<PlusNode>(move(node)));
  } else if (instanceof<BinaryOperationNode>(node.get())) {
    return visit_BinaryOperationNode(cast_node<BinaryOperationNode>(move(node)));
  } else if (instanceof<StringNode>(node.get())) {
    return visit_StringNode(cast_node<StringNode>(move(node)));
  } else if (instanceof<VarAssignmentNode>(node.get())) {
    return visit_VarAssignmentNode(cast_node<VarAssignmentNode>(move(node)));
  } else if (instanceof<DefineConstantNode>(node.get())) {
    return visit_DefineConstantNode(cast_node<DefineConstantNode>(move(node)));
  } else if (instanceof<VarAccessNode>(node.get())) {
    return visit_VarAccessNode(cast_node<VarAccessNode>(move(node)));
  } else if (instanceof<VarModifyNode>(node.get())) {
    return visit_VarModifyNode(cast_node<VarModifyNode>(move(node)));
  } else if (instanceof<BooleanNode>(node.get())) {
    return visit_BooleanNode(cast_node<BooleanNode>(move(node)));
  }
  throw UndefinedBehaviorException("Unimplemented visit method for input node '" + node->to_string() + "'");
}

/*
*
* Helper methods
*
*/

void Interpreter::populate(Value& value, const Position& pos_start, const Position& pos_end, shared_ptr<const Context> ctx) {
  value.set_pos(pos_start, pos_end);
  value.set_ctx(ctx);
}

void Interpreter::populate(Value& value, unique_ptr<const CustomNode> node, shared_ptr<const Context> ctx) {
  value.set_pos(node->getStartingPosition(), node->getEndingPosition());
  value.set_ctx(ctx);
}

void Interpreter::illegal_operation(unique_ptr<const CustomNode> node, shared_ptr<const Context> ctx) {
  throw RuntimeError(
    node->getStartingPosition(), node->getEndingPosition(),
    "Illegal operation",
    ctx
  );
}

void Interpreter::type_error(shared_ptr<const Value> value, const Type expected_type, shared_ptr<const Context> ctx) {
  throw TypeError(
    *(value->get_pos_start()), *(value->get_pos_end()),
    "Type '" + get_type_name(value->get_type()) + "' is not assignable to type '" + get_type_name(expected_type) + "'",
    ctx
  );
}

void Interpreter::make_success(unique_ptr<RuntimeResult>& res, unique_ptr<Value> value, unique_ptr<const CustomNode> node) {
  populate(*value, move(node), shared_ctx);
  res->success(move(value));
}

/*
*
* Visit methods
*
*/

unique_ptr<RuntimeResult> Interpreter::visit_ListNode(unique_ptr<ListNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  list<shared_ptr<const Value>> elements;
  if (node->get_number_of_nodes() > 0) {
    auto nodes = node->get_element_nodes();
    for (auto& element_node : *nodes) {
      shared_ptr<const Value> value = res->read(visit(move(element_node)));
      if (res->should_return()) return res;
      elements.push_back(value);
    }
  }
  unique_ptr<ListValue> list_value = make_unique<ListValue>(elements);
  make_success(res, move(list_value), move(node));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_IntegerNode(unique_ptr<const IntegerNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  int actual_integer;
  try {
    actual_integer = stoi(node->get_token().getStringValue());
  } catch (std::out_of_range e) {
    throw TypeOverflowError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Cannot store such a big integer",
      shared_ctx
    );
  }
  unique_ptr<IntegerValue> i = make_unique<IntegerValue>(actual_integer);
  make_success(res, move(i), move(node));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_DoubleNode(unique_ptr<const DoubleNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  double actual_double;
  try {
    actual_double = stod(node->get_token().getStringValue());
  } catch (std::out_of_range e) {
    throw TypeOverflowError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Cannot store such a big double",
      shared_ctx
    );
  }
  unique_ptr<DoubleValue> d = make_unique<DoubleValue>(actual_double);
  make_success(res, move(d), move(node));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_MinusNode(unique_ptr<MinusNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  shared_ptr<const Value> value = res->read(visit(node->get_node()));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(value)) {
    shared_ptr<const IntegerValue> integer = cast_const_value<IntegerValue>(value);
    unique_ptr<IntegerValue> negative_integer = make_unique<IntegerValue>(-1 * integer->get_actual_value());
    make_success(res, move(negative_integer), move(node));
    return res;
  } else if (instanceof<DoubleValue>(value.get())) {
    shared_ptr<const DoubleValue> d = cast_const_value<DoubleValue>(value);
    unique_ptr<DoubleValue> negative_double = make_unique<DoubleValue>(-1 * d->get_actual_value());
    make_success(res, move(negative_double), move(node));
    return res;
  }

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<RuntimeResult> Interpreter::visit_PlusNode(unique_ptr<PlusNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  shared_ptr<const Value> value = res->read(visit(node->get_node()));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(value)) {
    shared_ptr<const IntegerValue> integer = cast_const_value<IntegerValue>(value);
    unique_ptr<IntegerValue> positive_integer = make_unique<IntegerValue>(abs(integer->get_actual_value()));
    make_success(res, move(positive_integer), move(node));
    return res;
  } else if (instanceof<DoubleValue>(value)) {
    shared_ptr<const DoubleValue> d = cast_const_value<DoubleValue>(value);
    unique_ptr<DoubleValue> positive_double = make_unique<DoubleValue>(abs(d->get_actual_value()));
    make_success(res, move(positive_double), move(node));
    return res;
  }

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<Value> Interpreter::interpret_addition(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
  // The permutations:
  // - int + int = int
  // - int + double = double
  // - double + double = double
  // - double + int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_addition<IntegerValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_addition<IntegerValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_addition<DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_addition<DoubleValue, IntegerValue>(left, right, move(node));

  // Since concatenation is possible with any type of value,
  // it must be treated differently than the other types of additions.
  if (instanceof<StringValue>(left)) {
    shared_ptr<const StringValue> a = cast_const_value<StringValue>(left);
    unique_ptr<StringValue> concatenation = unique_ptr<StringValue>(*a + *right);
    populate(*concatenation, move(node), shared_ctx);
    return concatenation;
  } else if (instanceof<StringValue>(right)) {
    shared_ptr<const StringValue> b = cast_const_value<StringValue>(right);
    unique_ptr<StringValue> concatenation = unique_ptr<StringValue>(StringValue::make_concatenation_rtl(left.get(), b.get()));
    populate(*concatenation, move(node), shared_ctx);
    return concatenation;
  }

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<Value> Interpreter::interpret_substraction(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
  // The permutations:
  // - int - int = int
  // - int - double = double
  // - double - double = double
  // - double - int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_substraction<IntegerValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_substraction<IntegerValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_substraction<DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_substraction<DoubleValue, IntegerValue>(left, right, move(node));
  
  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<Value> Interpreter::interpret_multiplication(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
  // The permutations:
  // - int * int = int
  // - int * double = double
  // - double * double = double
  // - double * int = double
  // - string * int = string
  // - int * string = string
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_multiplication<IntegerValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_multiplication<IntegerValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_multiplication<DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_multiplication<DoubleValue, IntegerValue>(left, right, move(node));
  else if (instanceof<StringValue>(left)  && instanceof<IntegerValue>(right)) return make_multiplication<StringValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left)  && instanceof<StringValue>(right)) return make_multiplication<StringValue, IntegerValue>(right, left, move(node)); // we inverse the operation because it comes to the same thing, but as a consequence it cannot be tested in values.test.cpp

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<Value> Interpreter::interpret_power(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
  // The permutations:
  // - int ** int = int
  // - int ** double = double
  // - double ** double = double
  // - double ** int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_power<IntegerValue, IntegerValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_power<IntegerValue, DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_power<DoubleValue, DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_power<DoubleValue, IntegerValue, DoubleValue>(left, right, move(node));

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<Value> Interpreter::interpret_division(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
  // The permutations:
  // - int / int = int
  // - int / double = double
  // - double / double = double
  // - double / int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_division<IntegerValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_division<IntegerValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_division<DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_division<DoubleValue, IntegerValue>(left, right, move(node));

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<Value> Interpreter::interpret_modulo(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
  // The permutations:
  // - int % int = int
  // - int % double = double
  // - double % double = double
  // - double % int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_modulo<IntegerValue, IntegerValue>(left, right, move(node));
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_modulo<IntegerValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_modulo<DoubleValue, DoubleValue>(left, right, move(node));
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_modulo<DoubleValue, IntegerValue>(left, right, move(node));

  illegal_operation(move(node), shared_ctx);
  return nullptr;
}

unique_ptr<RuntimeResult> Interpreter::visit_BinaryOperationNode(unique_ptr<BinaryOperationNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  shared_ptr<const Value> left = res->read(visit(node->retrieve_a()));
  if (res->should_return()) return res;
  shared_ptr<const Value> right = res->read(visit(node->retrieve_b()));
  if (res->should_return()) return res;

  // A boolean, when used in mathematical operations should be considered as an Integer.
  // - true = 1
  // - false = 0
  if (instanceof<BooleanValue>(left)) left = left->cast(Type::INT);
  if (instanceof<BooleanValue>(right)) right = right->cast(Type::INT);

  if      (instanceof<AddNode>(node))       res->success(interpret_addition(move(left), move(right), move(node)));
  else if (instanceof<SubstractNode>(node)) res->success(interpret_substraction(move(left), move(right), move(node)));
  else if (instanceof<MultiplyNode>(node))  res->success(interpret_multiplication(move(left), move(right), move(node)));
  else if (instanceof<PowerNode>(node))     res->success(interpret_power(move(left), move(right), move(node)));
  else if (instanceof<DivideNode>(node))    res->success(interpret_division(move(left), move(right), move(node)));
  else if (instanceof<ModuloNode>(node))    res->success(interpret_modulo(move(left), move(right), move(node)));
  else illegal_operation(move(node), shared_ctx);

  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_VarAssignmentNode(unique_ptr<VarAssignmentNode> node) {
  const string& variable_name = node->get_var_name();
  if (shared_ctx->get_symbol_table()->exists(variable_name)) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "The variable named '" + variable_name + "' already defined in the current context.",
      shared_ctx
    );
  }

  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  bool has_initial_value = node->has_value(); // because "retrieve_value_node()" will change the result of this method
  shared_ptr<Value> initial_value = has_initial_value ? res->read(visit(node->retrieve_value_node())) : nullptr;
  if (res->should_return()) return res;

  // A default value must be assigned
  // if the developer didn't set an initial value.
  // This default value will depend on the given type.
  if (!has_initial_value) {
    switch (node->get_type()) {
      case Type::INT: initial_value = make_shared<IntegerValue>(); break;
      case Type::DOUBLE: initial_value = make_shared<DoubleValue>(); break;
      case Type::STRING: initial_value = make_shared<StringValue>(); break;
      default:
        throw RuntimeError(
          node->getStartingPosition(), node->getEndingPosition(),
          "The variable named '" + variable_name + "' cannot receive a default value for this type.",
          shared_ctx
        );
    }
  } else {
    if (node->get_type() != initial_value->get_type()) {
      shared_ptr<Value> cast_value = initial_value->cast(node->get_type());
      if (cast_value == nullptr) {
        type_error(
          initial_value,
          node->get_type(),
          shared_ctx
        );
      }
      initial_value = cast_value;
    }
  }

  populate(*initial_value, move(node), shared_ctx);
  shared_ctx->get_symbol_table()->set(variable_name, unique_ptr<Value>(initial_value->copy()), false); // copy's important because the garbage collector deallocates the returning value

  res->success(unique_ptr<Value>(initial_value->copy()));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_DefineConstantNode(unique_ptr<DefineConstantNode> node) {
  // TODO: a constant cannot be created in a nested context

  const string& variable_name = node->get_var_name();
  if (shared_ctx->get_symbol_table()->exists(variable_name)) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "The constant named '" + variable_name + "' already defined.",
      shared_ctx
    );
  }

  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  shared_ptr<Value> value = res->read(visit(node->retrieve_value_node()));
  if (res->should_return()) return res;

  if (node->get_type() != value->get_type()) {
    shared_ptr<Value> cast_value = value->cast(node->get_type());
    if (cast_value == nullptr) {
      type_error(
        value,
        node->get_type(),
        shared_ctx
      );
    }
    value = cast_value;
  }

  populate(*value, move(node), shared_ctx);
  shared_ctx->get_symbol_table()->set(variable_name, unique_ptr<Value>(value->copy()), true);

  res->success(unique_ptr<Value>(value->copy()));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_VarAccessNode(unique_ptr<VarAccessNode> node) {
  const string& variable_name = node->get_var_name();
  if (!shared_ctx->get_symbol_table()->exists_globally(variable_name)) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Undefined variable '" + variable_name + "'.",
      shared_ctx
    );
  }
  
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  unique_ptr<Value> value = shared_ctx->get_symbol_table()->get(variable_name); // "get" returns a copy of the variable stored in the symbol table
  make_success(res, move(value), move(node));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_VarModifyNode(unique_ptr<VarModifyNode> node) {
  const string& variable_name = node->get_var_name();
  if (!shared_ctx->get_symbol_table()->exists_globally(variable_name)) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Undefined variable '" + variable_name + "'.",
      shared_ctx
    );
  }

  if (shared_ctx->get_symbol_table()->is_constant(variable_name)) {
    throw TypeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Assignment to constant variable",
      shared_ctx
    );
  }

  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  shared_ptr<Value> new_value = res->read(visit(node->get_value_node()));
  if (res->should_return()) return res;

  // If the type isn't exactly the same,
  // then try to cast the given value
  // so as to match the one of the variable.
  // If it doesn't work, throw a TypeError.
  unique_ptr<Value> existing_value = shared_ctx->get_symbol_table()->get(variable_name);
  if (new_value->get_type() != existing_value->get_type()) {
    shared_ptr<Value> cast_value = new_value->cast(existing_value->get_type());
    if (cast_value == nullptr) {
      type_error(
        new_value,
        existing_value->get_type(),
        shared_ctx
      );
    }
    new_value = cast_value;
  }

  shared_ctx->get_symbol_table()->modify(variable_name, unique_ptr<Value>(new_value->copy()));

  // It's important to keep in mind that the garbage collector will deallocate the returned value of a statement.
  // To make sure it doesn't delete a variable, it must return a copy.
  res->success(unique_ptr<Value>(new_value->copy()));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_StringNode(unique_ptr<const StringNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  unique_ptr<StringValue> str = make_unique<StringValue>(node->getValue());
  make_success(res, move(str), move(node));
  return res;
}

unique_ptr<RuntimeResult> Interpreter::visit_BooleanNode(unique_ptr<const BooleanNode> node) {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  unique_ptr<BooleanValue> str = make_unique<BooleanValue>(node->is_true());
  make_success(res, move(str), move(node));
  return res;
}
