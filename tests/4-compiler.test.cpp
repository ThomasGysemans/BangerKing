#include <iostream>
#include <cstdio>
#include <list>
#include "doctest.h"
#include "../include/compiler.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/compiler/compiler_error.hpp"
#include "../include/utils/read_entire_file.hpp"
using namespace std;

static const string output_file_path = "tests_output_file.s";

unique_ptr<CustomNode> make_integer_node(const string& str_value) {
  const auto token = Token(TokenType::NUMBER, str_value, Position::getDefaultPos());
  return make_unique<IntegerNode>(token);
}

unique_ptr<CustomNode> make_double_node(const string& str_value) {
  const auto token = Token(TokenType::NUMBER, str_value, Position::getDefaultPos());
  return make_unique<DoubleNode>(token);
}

unique_ptr<CustomNode> make_string_node(const string& str_value) {
  const auto token = Token(TokenType::STR, str_value, Position::getDefaultPos(), nullptr, true);
  return make_unique<StringNode>(token);
}

unique_ptr<DefineConstantNode> make_constant(const string& name, unique_ptr<CustomNode>&& value_node) {
  return make_unique<DefineConstantNode>(name, move(value_node), Type::INT, Position::getDefaultPos(), Position::getDefaultPos());
}

unique_ptr<VarAssignmentNode> make_variable(const string& name, unique_ptr<CustomNode>&& value_node) {
  return make_unique<VarAssignmentNode>(name, move(value_node), Token(TokenType::IDENTIFIER, "int", Position::getDefaultPos()), Position::getDefaultPos(), Position::getDefaultPos());
}

unique_ptr<VarAccessNode> make_access_to_variable(const string& name) {
  return make_unique<VarAccessNode>(Token(TokenType::IDENTIFIER, name, Position::getDefaultPos()));
}

template <typename T>
unique_ptr<T> make_bin_op_between_literals() {
  auto a = make_integer_node("5");
  auto b = make_integer_node("6");
  return make_unique<T>(a, b);
}

template <typename T>
unique_ptr<T> make_bin_op_with_one_literal(const string& access_variable = "a", const string& literal = "6") {
  auto a = cast_node<CustomNode>(make_access_to_variable(access_variable));
  auto b = make_integer_node(literal);
  return make_unique<T>(a, b);
}

template <typename T>
unique_ptr<T> make_bin_op_with_no_literal() {
  auto a = cast_node<CustomNode>(make_access_to_variable("a"));
  auto b = cast_node<CustomNode>(make_access_to_variable("b"));
  return make_unique<T>(a, b);
}

void init_empty_compiler() {
  auto nodes = make_unique<list<unique_ptr<CustomNode>>>();
  auto ast = make_unique<ListNode>(move(nodes));
  Compiler::compile(move(ast), output_file_path);
}

void clear_compiler() {
  Compiler::clear(output_file_path);
}

void clear_compiler(const string& path) {
  Compiler::clear(path);
}

void delete_output_file() {
  remove(output_file_path.c_str());
}

DOCTEST_TEST_SUITE("Compiler helper functions") {
  SCENARIO("make integer node") {
    auto node = make_integer_node("5");
    auto integer = cast_node<IntegerNode>(move(node));
    CHECK(integer->getNodeType() == NodeType::INTEGER);
    CHECK(integer->get_token().getStringValue() == "5");
  }

  SCENARIO("make double node") {
    auto node = make_double_node("5.0");
    auto d = cast_node<DoubleNode>(move(node));
    CHECK(d->getNodeType() == NodeType::DOUBLE);
    CHECK(d->get_token().getStringValue() == "5.0");
  }

  SCENARIO("make string node") {
    auto node = make_string_node("hello");
    auto str = cast_node<StringNode>(move(node));
    CHECK(str->getNodeType() == NodeType::STRING);
    CHECK(str->canConcatenate());
    CHECK(str->getValue() == "hello");
  }

  SCENARIO("make variable assignment") {
    const auto var = make_variable("hello", make_integer_node("5"));
    CHECK(var->getNodeType() == NodeType::VAR_ASSIGNMENT);
    CHECK(var->get_var_name() == "hello");
    CHECK(var->has_value());
    CHECK(var->get_type() == Type::INT);
    CHECK(var->retrieve_value_node()->getNodeType() == NodeType::INTEGER);
  }

  SCENARIO("make access to variable") {
    auto access = make_access_to_variable("hello");
    CHECK(access->getNodeType() == NodeType::VAR_ACCESS);
    CHECK(access->get_var_name() == "hello");
  }

  SCENARIO("make binary operation") {
    const auto addition_2_literals = make_bin_op_between_literals<AddNode>();
    const auto subtraction_2_literals = make_bin_op_between_literals<SubstractNode>();
    const auto multiplication_2_literals = make_bin_op_between_literals<MultiplyNode>();
    CHECK(addition_2_literals->getNodeType() == NodeType::ADD);
    CHECK(subtraction_2_literals->getNodeType() == NodeType::SUBSTRACT);
    CHECK(multiplication_2_literals->getNodeType() == NodeType::MULTIPLY);
    const auto addition_1_literal = make_bin_op_with_one_literal<AddNode>();
    const auto subtraction_1_literal = make_bin_op_with_one_literal<SubstractNode>();
    const auto multiplication_1_literal = make_bin_op_with_one_literal<MultiplyNode>();
    CHECK(addition_1_literal->getNodeType() == NodeType::ADD);
    CHECK(subtraction_1_literal->getNodeType() == NodeType::SUBSTRACT);
    CHECK(multiplication_1_literal->getNodeType() == NodeType::MULTIPLY);
    const auto addition_0_literal = make_bin_op_with_one_literal<AddNode>();
    const auto subtraction_0_literal = make_bin_op_with_one_literal<SubstractNode>();
    const auto multiplication_0_literal = make_bin_op_with_one_literal<MultiplyNode>();
    CHECK(addition_0_literal->getNodeType() == NodeType::ADD);
    CHECK(subtraction_0_literal->getNodeType() == NodeType::SUBSTRACT);
    CHECK(multiplication_0_literal->getNodeType() == NodeType::MULTIPLY);
  }

  SCENARIO("start empty compiler & clear it") {
    CHECK_NOTHROW(init_empty_compiler());
    CHECK_NOTHROW(clear_compiler());
    CHECK_NOTHROW(delete_output_file());
  }
}

DOCTEST_TEST_SUITE("Compiler operations") {
  SCENARIO("assignment with one literal") {
    CHECK(!CompilerOperations::is_assignment_with_one_literal(CompilerOperations::LITERAL));
    CHECK(CompilerOperations::is_assignment_with_one_literal(CompilerOperations::VAR_ADD_LITERAL));
    CHECK(!CompilerOperations::is_assignment_with_one_literal(CompilerOperations::ADD_LITERALS));
  }

  SCENARIO("assignment with two literals") {
    CHECK(!CompilerOperations::is_assignment_with_two_literals(CompilerOperations::LITERAL));
    CHECK(!CompilerOperations::is_assignment_with_two_literals(CompilerOperations::VAR_ADD_LITERAL));
    CHECK(CompilerOperations::is_assignment_with_two_literals(CompilerOperations::ADD_LITERALS));
  }

  SCENARIO("get assembly operation") {
    CHECK_THROWS_AS(CompilerOperations::get_assembly_operation(CompilerOperations::LITERAL), UndefinedBehaviorException);
    CHECK(CompilerOperations::get_assembly_operation(CompilerOperations::VAR_ADD_LITERAL) == "adds");
    CHECK(CompilerOperations::get_assembly_operation(CompilerOperations::ADD_LITERALS) == "adds");
    CHECK(CompilerOperations::get_assembly_operation(CompilerOperations::VAR_SUB_LITERAL) == "subs");
    CHECK(CompilerOperations::get_assembly_operation(CompilerOperations::SUB_LITERALS) == "subs");
    CHECK(CompilerOperations::get_assembly_operation(CompilerOperations::VAR_MUL_LITERAL) == "mul");
    CHECK(CompilerOperations::get_assembly_operation(CompilerOperations::MUL_LITERALS) == "mul");
  }
}

// Tests some functions of the compiler individually
DOCTEST_TEST_SUITE("Simple compiler") {
  SCENARIO("init tests") {
    CHECK_NOTHROW(init_empty_compiler());
  }

  SCENARIO("is literal") {
    const auto integer = make_integer_node("5");
    const auto d = make_double_node("5.0");
    const auto str = make_string_node("hello");
    CHECK(Compiler::is_literal(integer.get()));
    CHECK(Compiler::is_literal(d.get()));
    CHECK(!Compiler::is_literal(str.get()));
  }

  SCENARIO("is binary operation") {
    auto a = make_integer_node("5");
    auto b = make_integer_node("6");
    auto bin_op = make_unique<AddNode>(a, b);
    CHECK(Compiler::is_binary_op(bin_op.get()));
    CHECK(!Compiler::is_binary_op(a.get()));
    CHECK(!Compiler::is_binary_op(b.get()));
  }

  SCENARIO("get type of binary operation") {
    CHECK(Compiler::get_type_of_bin_op(make_bin_op_between_literals<AddNode>().get(), true) == CompilerOperations::VariableAssignment::ADD_LITERALS);
    CHECK(Compiler::get_type_of_bin_op(make_bin_op_between_literals<SubstractNode>().get(), true) == CompilerOperations::VariableAssignment::SUB_LITERALS);
    CHECK(Compiler::get_type_of_bin_op(make_bin_op_between_literals<MultiplyNode>().get(), true) == CompilerOperations::VariableAssignment::MUL_LITERALS);
    CHECK(Compiler::get_type_of_bin_op(make_bin_op_with_one_literal<AddNode>().get(), false) == CompilerOperations::VariableAssignment::VAR_ADD_LITERAL);
    CHECK(Compiler::get_type_of_bin_op(make_bin_op_with_one_literal<SubstractNode>().get(), false) == CompilerOperations::VariableAssignment::VAR_SUB_LITERAL);
    CHECK(Compiler::get_type_of_bin_op(make_bin_op_with_one_literal<MultiplyNode>().get(), false) == CompilerOperations::VariableAssignment::VAR_MUL_LITERAL);
  }

  SCENARIO("get assembly var type") {
    CHECK(Compiler::get_assembly_var_type(Type::INT) == ".word");
    CHECK(Compiler::get_assembly_var_type(Type::STRING) == ".ascii");
    CHECK_THROWS_AS(Compiler::get_assembly_var_type(Type::DOUBLE), UndefinedBehaviorException);
  }

  SCENARIO("init constant stack variable") {
    auto a = make_integer_node("5");
    CHECK_THROWS_AS(Compiler::require_variable("a", Position::getDefaultPos(), Position::getDefaultPos()), CompilerError);
    CHECK_NOTHROW(Compiler::require_not_existing_variable("a", Position::getDefaultPos(), Position::getDefaultPos()));
    CHECK_NOTHROW(Compiler::init_constant_stack_variable("a", Type::INT, move(a)));
    const auto& storage_a = Compiler::find("a", Position::getDefaultPos(), Position::getDefaultPos());
    CHECK(storage_a.addr == 0);
    CHECK(storage_a.is_register == false);
    CHECK_THROWS_AS(Compiler::init_constant_stack_variable("a", Type::INT, make_integer_node("5")), CompilerError);
    CHECK_THROWS_AS(Compiler::require_not_existing_variable("a", Position::getDefaultPos(), Position::getDefaultPos()), CompilerError);
    CHECK_NOTHROW(Compiler::require_variable("a", Position::getDefaultPos(), Position::getDefaultPos()));
    clear_compiler();
  }

  SCENARIO("assign register") {
    auto a = make_integer_node("5");
    Compiler::init_constant_stack_variable("a", Type::INT, move(a));
    Compiler::assign_register(0, "a");
    storage_t& storage = Compiler::find("a", Position::getDefaultPos(), Position::getDefaultPos());
    CHECK(storage.addr == 0);
    CHECK(!storage.is_register); // there was no explicit action that moved the stack to the register, so it's false for now
    clear_compiler();
  }

  SCENARIO("lock registers") {
    auto a = make_integer_node("5");
    Compiler::init_constant_stack_variable("a", Type::INT, move(a));
    Compiler::assign_register(0, "a");
    Compiler::lock_register(0);
    CHECK(Compiler::is_register_locked(0));
    Compiler::release_register(0);
    CHECK(!Compiler::is_register_locked(0));
    CHECK_THROWS_AS(Compiler::release_register(0), Exception);
    clear_compiler();
  }

  SCENARIO("get a free register and lock it") {
    auto reg = Compiler::get_free_register();
    CHECK(Compiler::is_register_locked(reg));
    auto reg2 = Compiler::get_free_register();
    CHECK(Compiler::is_register_locked(reg2));
    CHECK(reg != reg2);
    clear_compiler();
  }

  SCENARIO("clean tests") {
    CHECK_NOTHROW(clear_compiler());
    CHECK_NOTHROW(delete_output_file());
  }
}

using list_nodes = unique_ptr<list<unique_ptr<CustomNode>>>;

list_nodes get_empty_ast() {
  return make_unique<list<unique_ptr<CustomNode>>>();
}

void compile_with_nodes(list_nodes&& nodes) {
  auto ast = make_unique<ListNode>(move(nodes));
  Compiler::compile(move(ast), output_file_path);
}

bool expect(const string& expected_content) {
  const string content = read_entire_file(output_file_path);
  // cout << "expected content \"" << expected_content << '"' << endl;
  // cout << "but got \"" << content << '"' << endl;
  return content == expected_content;
}

bool expect_start_label(const string& expected_label) {
  const string content = read_entire_file(output_file_path);
  string top = content.substr(0, content.find("\n.data"));
  remove_substring(top, "\t");
  remove_substring(top, ".global _start\n_start:\n");
  // cout << "top = '" << top << "'" << endl;
  // cout << "expected_label = '" << expected_label << "'" << endl;
  return top == expected_label;
}

bool expect_data_section(const string& expected_data) {
  const string content = read_entire_file(output_file_path);
  string top = content.substr(content.find("\n.data") + 7 ); // 7 to remove the \n too
  remove_substring(top, "\t");
  // cout << "top = \"" << top << '"' << endl;
  // cout << "expected_data = \"" << expected_data << '"' << endl;
  return top == expected_data;
}

DOCTEST_TEST_SUITE("Compiler output file") {
  SCENARIO("empty AST") {
    auto nodes = make_unique<list<unique_ptr<CustomNode>>>();
    auto ast = make_unique<ListNode>(move(nodes));
    Compiler::compile(move(ast), output_file_path);
    CHECK(expect(".global _start\n_start:\n\tswi 0\n"));
    CHECK(expect_start_label("swi 0\n"));
    clear_compiler();
  }

  SCENARIO("literal to be saved in the data section") {
    auto nodes = get_empty_ast();
    nodes->push_back(make_variable("a", make_integer_node("5")));
    compile_with_nodes(move(nodes));

    CHECK(expect(".global _start\n_start:\n\tswi 0\n\n.data\n\ta: .word 5\n"));
    CHECK(expect_start_label("swi 0\n"));
    CHECK(expect_data_section("a: .word 5\n"));
    clear_compiler();
  }

  SCENARIO("constant with literal") {
    auto nodes = get_empty_ast();
    nodes->push_back(make_constant("a", make_integer_node("6")));
    compile_with_nodes(move(nodes));

    CHECK(expect_data_section("a: .word 6\n"));
    CHECK(expect_start_label("swi 0\n"));
    clear_compiler();
  }

  SCENARIO("multiple variables of literals") {
    auto nodes = get_empty_ast();
    nodes->push_back(make_variable("a", make_integer_node("1")));
    nodes->push_back(make_variable("b", make_integer_node("2")));
    nodes->push_back(make_variable("c", make_integer_node("3")));
    compile_with_nodes(move(nodes));

    CHECK(expect_data_section("a: .word 1\nb: .word 2\nc: .word 3\n"));
    CHECK(expect_start_label("swi 0\n"));
    clear_compiler();
  }

  SCENARIO("variable whose value includes one variable and one literal") {
    auto nodes = get_empty_ast();
    nodes->push_back(make_variable("a", make_integer_node("5"))); // store a as int = 5
    nodes->push_back(make_variable("b", make_bin_op_with_one_literal<AddNode>("a", "9"))); // store b as int = a + 9
    compile_with_nodes(move(nodes));

    CHECK(expect_data_section("a: .word 5\n"));
    CHECK(expect_start_label("ldr r1, =a\nldr r1, [r1]\nadds r0, r1, #9\npush {r0}\nswi 0\n"));
    CHECK(!Compiler::is_register_locked(0));
    CHECK(!Compiler::is_register_locked(1));
    clear_compiler();
  }

  SCENARIO("delete output file") {
    CHECK_NOTHROW(remove(output_file_path.c_str()));
  }
}