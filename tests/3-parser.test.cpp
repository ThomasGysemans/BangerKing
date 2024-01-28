#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/token.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/nodes/compositer.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/debug/print_tokens.hpp"
using namespace std;

unique_ptr<list<unique_ptr<CustomNode>>> get_element_nodes_from(const string& code, bool debug_print = false) {
  Lexer lexer(&code);
  READ_FILES.insert({ "<stdin>", make_unique<string>(code) });
  list<unique_ptr<const Token>> tokens = lexer.generate_tokens();
  if (debug_print) {
    cout << "Resulft of lexer :" << endl;
    cout << display_tokens_list(tokens) << endl;
  }
  Parser parser(tokens);
  unique_ptr<ListNode> parsing_result = parser.parse();
  if (debug_print) {
    cout << "Result of parsing :" << endl;
    cout << parsing_result->to_string() << endl;
  }
  return parsing_result->get_element_nodes();
}

template <typename E>
bool should_throw(const string& code) {
  try {
    get_element_nodes_from(code);
    return false;
  } catch (E e) {
    return true;
  }
}

void test_simple_number() {
  const auto element_nodes = get_element_nodes_from("5");
  const auto number_node = cast_node<IntegerNode>(move(element_nodes->front()));

  assert(element_nodes->size() == 1);
  assert(number_node->get_token().getStringValue() == "5");

  print_success_msg("parser with simple number", 1);
}

void test_simple_decimal_number() {
  const auto element_nodes = get_element_nodes_from("3.14");
  const auto number_node = cast_node<DoubleNode>(move(element_nodes->front()));

  assert(element_nodes->size() == 1);
  assert(number_node->get_token().getStringValue() == "3.14");
  assert(number_node->to_string() == "DoubleNode(3.14)"); // to make sure the double is printed out correctly

  print_success_msg("parser with simple decimal number", 1);
}

void test_simple_negative_number() {
  const auto nodes = get_element_nodes_from("-5");
  const auto minus_node = cast_node<MinusNode>(move(nodes->front()));
  const auto number_node = cast_node<IntegerNode>(minus_node->get_node());
  assert(nodes->size() == 1);
  assert(number_node->get_token().getStringValue() == "5");
  
  print_success_msg("parser with simple negative number", 1);
}

void test_simple_positive_number() {
  const auto nodes = get_element_nodes_from("+670");
  const auto plus_node = cast_node<PlusNode>(move(nodes->front()));
  const auto number_node = cast_node<IntegerNode>(plus_node->get_node());
  assert(nodes->size() == 1);
  assert(number_node->get_token().getStringValue() == "670");

  print_success_msg("parser with simple positive number", 1);
}

void test_simple_addition() {
  const auto nodes = get_element_nodes_from("5+6");
  const auto add_node = cast_node<AddNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(add_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(add_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "5");
  assert(b->get_token().getStringValue() == "6");

  print_success_msg("parser with simple addition", 1);
}

void test_simple_addition_with_whitespace() {
  const auto nodes = get_element_nodes_from("5 + 6");
  const auto add_node = cast_node<AddNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(add_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(add_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "5");
  assert(b->get_token().getStringValue() == "6");

  print_success_msg("parser with simple addition and whitespace", 1);
}

void test_simple_substraction() {
  const auto nodes = get_element_nodes_from("0-6");
  const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(sub_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(sub_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "0");
  assert(b->get_token().getStringValue() == "6");

  print_success_msg("parser with simple substraction", 1);
}

void test_simple_substraction_with_whitespace() {
  const auto nodes = get_element_nodes_from("0 - 6");
  const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(sub_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(sub_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "0");
  assert(b->get_token().getStringValue() == "6");

  print_success_msg("parser with simple substraction and whitespace", 1);
}

void test_simple_multiplication() {
  const auto nodes = get_element_nodes_from("0*6");
  const auto mul_node = cast_node<MultiplyNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(mul_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(mul_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "0");
  assert(b->get_token().getStringValue() == "6");

  print_success_msg("parser with simple multiplication", 1);
}

void test_simple_divison() {
  const auto nodes = get_element_nodes_from("10/2");
  const auto div_node = cast_node<DivideNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(div_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(div_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "10");
  assert(b->get_token().getStringValue() == "2");

  print_success_msg("parser with simple divison", 1);
}

void test_simple_modulo() {
  const auto nodes = get_element_nodes_from("13%12");
  const auto modulo_node = cast_node<ModuloNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(modulo_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(modulo_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "13");
  assert(b->get_token().getStringValue() == "12");

  print_success_msg("parser with simple modulo", 1);
}

void test_simple_power() {
  const auto nodes = get_element_nodes_from("10**2");
  const auto power_node = cast_node<PowerNode>(move(nodes->front()));
  const auto a = cast_node<IntegerNode>(power_node->retrieve_a());
  const auto b = cast_node<IntegerNode>(power_node->retrieve_b());
  assert(nodes->size() == 1);
  assert(a->get_token().getStringValue() == "10");
  assert(b->get_token().getStringValue() == "2");

  print_success_msg("parser with simple power", 1);
}

void test_complex_maths_expression_with_parenthesis() {
  const auto nodes = get_element_nodes_from("10+(5-2)/6"); // AddNode ( IntegerNode(10), DivideNode ( SubstractNode, IntegerNode(6) ) )
  assert(nodes->size() == 1);

  const auto add_node = cast_node<AddNode>(move(nodes->front()));
  const auto add_a = cast_node<IntegerNode>(add_node->retrieve_a()); // 10
  const auto add_b = cast_node<DivideNode>(add_node->retrieve_b()); // DivideNode( SubstractNode(), IntegerNode(10) )
  assert(add_a->get_token().getStringValue() == "10");

  const auto sub_node = cast_node<SubstractNode>(add_b->retrieve_a());
  const auto sub_a = cast_node<IntegerNode>(sub_node->retrieve_a());
  const auto sub_b = cast_node<IntegerNode>(sub_node->retrieve_b());
  assert(sub_a->get_token().getStringValue() == "5");
  assert(sub_b->get_token().getStringValue() == "2");

  const auto den = cast_node<IntegerNode>(add_b->retrieve_b());
  assert(den->get_token().getStringValue() == "6");

  print_success_msg("parser with complex maths expression with parenthesis", 1);
}

void test_complex_maths_expression_without_parenthesis() {
  const auto nodes = get_element_nodes_from("10+5-2/6"); // SubstractNode( AddNode(10, 5), DivideNode(2, 6) )
  assert(nodes->size() == 1);

  const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
  const auto add_node = cast_node<AddNode>(sub_node->retrieve_a());
  const auto div_node = cast_node<DivideNode>(sub_node->retrieve_b());

  const auto add_a = cast_node<IntegerNode>(add_node->retrieve_a());
  const auto add_b = cast_node<IntegerNode>(add_node->retrieve_b());
  assert(add_a->get_token().getStringValue() == "10");
  assert(add_b->get_token().getStringValue() == "5");

  const auto div_a = cast_node<IntegerNode>(div_node->retrieve_a());
  const auto div_b = cast_node<IntegerNode>(div_node->retrieve_b());
  assert(div_a->get_token().getStringValue() == "2");
  assert(div_b->get_token().getStringValue() == "6");

  print_success_msg("parser with complex maths expression without parenthesis", 1);
}

void test_mutiple_lines() {
  const auto nodes = get_element_nodes_from(
    "1+2\n"
    "3-4"
  );
  assert(nodes->size() == 2);

  const auto first_node = cast_node<AddNode>(move(nodes->front()));
  const auto second_node = cast_node<SubstractNode>(move(nodes->back()));

  assert(cast_node<IntegerNode>(first_node->retrieve_a())->get_token().getStringValue() == "1");
  assert(cast_node<IntegerNode>(first_node->retrieve_b())->get_token().getStringValue() == "2");
  assert(cast_node<IntegerNode>(second_node->retrieve_a())->get_token().getStringValue() == "3");
  assert(cast_node<IntegerNode>(second_node->retrieve_b())->get_token().getStringValue() == "4");

  print_success_msg("parser with multiple lines", 1);
}

void test_positions_on_single_digit() {
  const auto nodes = get_element_nodes_from("5");
  assert(nodes->size() == 1);

  const auto node = cast_node<IntegerNode>(move(nodes->front()));
  const auto pos_start = node->getStartingPosition();
  const auto pos_end = node->getEndingPosition();

  assert(pos_start.get_idx() == 0);
  assert(pos_start.get_col() == 0);
  assert(pos_start.get_ln() == 0);

  assert(pos_end.get_idx() == 1);
  assert(pos_end.get_col() == 1);
  assert(pos_end.get_ln() == 0);

  print_success_msg("node's positions are valid on a single digit", 1);
}

void test_positions_on_math_expression() {
  const auto nodes = get_element_nodes_from("5+6-2");
  assert(nodes->size() == 1);

  const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
  const auto add_node = cast_node<AddNode>(sub_node->retrieve_a());
  const auto two = cast_node<IntegerNode>(sub_node->retrieve_b());

  const auto five = cast_node<IntegerNode>(add_node->retrieve_a());
  const auto six = cast_node<IntegerNode>(add_node->retrieve_b());

  assert(five->get_token().getStringValue() == "5");
  assert(six->get_token().getStringValue() == "6");
  assert(two->get_token().getStringValue() == "2");

  assert(five->getStartingPosition().get_idx() == 0);
  assert(five->getEndingPosition().get_idx() == 1);
  assert(six->getStartingPosition().get_idx() == 2);
  assert(six->getEndingPosition().get_idx() == 3);
  assert(two->getStartingPosition().get_idx() == 4);
  assert(two->getEndingPosition().get_idx() == 5);

  assert(sub_node->getStartingPosition().get_idx() == five->getStartingPosition().get_idx());
  assert(sub_node->getEndingPosition().get_idx() == two->getEndingPosition().get_idx());
  assert(add_node->getStartingPosition().get_idx() == five->getStartingPosition().get_idx());
  assert(add_node->getEndingPosition().get_idx() == six->getEndingPosition().get_idx());

  print_success_msg("node's positions are valid on a math expression", 1);
}

void test_variable_assignment_with_initial_value() {
  const string code = "store a as int = 5";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes->size() == 1);

  const auto node = cast_node<VarAssignmentNode>(move(nodes->front()));
  assert(node->get_type_name() == "int");
  assert(node->get_var_name() == "a");
  assert(node->has_value());

  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  unique_ptr<CustomNode> value_node = node->retrieve_value_node();
  assert(value_node->getStartingPosition().get_idx() == code.size() - 1);
  assert(value_node->getEndingPosition().get_idx() == code.size());

  print_success_msg("variable assignment of an integer with initial value", 1);
}

void test_variable_assignment_without_initial_value() {
  const string code = "store a as int";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes->size() == 1);

  const auto node = cast_node<VarAssignmentNode>(move(nodes->front()));
  assert(node->get_type_name() == "int");
  assert(node->get_var_name() == "a");
  assert(!node->has_value());

  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  print_success_msg("variable assignment of an integer without initial value", 1);
}

void test_variable_assignment_invalid_syntax() {
  try {
    get_element_nodes_from("store a as int = store");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("store a as int 8");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("store as int");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("store a");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("store");
    assert(false);
  } catch (InvalidSyntaxError e) {}
  
  print_success_msg("throws an error if the syntax of a variable assignment isn't right");
}

void test_variable_modification() {
  const string code = "a = 5";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes->size() == 1);

  const auto var_modify_node = cast_node<VarModifyNode>(move(nodes->front()));
  assert(var_modify_node->get_var_name() == "a");

  const auto integer = cast_node<IntegerNode>(var_modify_node->get_value_node());
  assert(integer->get_token().getStringValue() == "5");

  assert(var_modify_node->getStartingPosition().get_idx() == 0);
  assert(var_modify_node->getEndingPosition().get_idx() == code.size());
  assert(integer->getStartingPosition().get_idx() == 4);
  assert(integer->getEndingPosition().get_idx() == 5);

  print_success_msg("variable modification", 1);
}

void test_access_node() {
  const string code = "variable";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes->size() == 1);

  const auto node = cast_node<VarAccessNode>(move(nodes->front()));
  assert(node->get_var_name() == "variable");
  assert(node->to_string() == "(variable)");
  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  print_success_msg("variable access", 1);
}

void test_access_node_in_expression() {
  const string code = "a+5";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes->size() == 1);

  const auto node = cast_node<AddNode>(move(nodes->front()));
  assert(instanceof<VarAccessNode>(node->retrieve_a().get()));
  assert(instanceof<IntegerNode>(node->retrieve_b().get()));
  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  print_success_msg("variable access in maths expression", 1);
}

void test_string() {
  const auto double_quote_string = cast_node<StringNode>(move(get_element_nodes_from("\"hello\"")->front()));
  assert(double_quote_string->canConcatenate());
  assert(double_quote_string->getToken()->canConcatenate());
  assert(double_quote_string->getToken()->ofType(TokenType::STR));
  assert(double_quote_string->getValue() == "hello");

  const auto simple_quote_string = cast_node<StringNode>(move(get_element_nodes_from("'YOYO'")->front()));
  assert(!simple_quote_string->canConcatenate());
  assert(!simple_quote_string->getToken()->canConcatenate());
  assert(simple_quote_string->getToken()->ofType(TokenType::STR));
  assert(simple_quote_string->getValue() == "YOYO");

  const auto escaped_backslash = cast_node<StringNode>(move(get_element_nodes_from("'C\\'EST'")->front()));
  assert(!escaped_backslash->canConcatenate());
  assert(!escaped_backslash->getToken()->canConcatenate());
  assert(escaped_backslash->getToken()->ofType(TokenType::STR));
  assert(escaped_backslash->getValue() == "C'EST");

  print_success_msg("strings with simple, double quotes and backslashes", 1);
}

void test_boolean() {
  const auto t = cast_node<BooleanNode>(move(get_element_nodes_from("true")->front()));
  const auto f = cast_node<BooleanNode>(move(get_element_nodes_from("false")->front()));
  
  assert(!t->getToken()->canConcatenate());
  assert(t->getToken()->ofType(TokenType::KEYWORD));
  assert(t->getToken()->is_keyword("true"));

  assert(!f->getToken()->canConcatenate());
  assert(f->getToken()->ofType(TokenType::KEYWORD));
  assert(f->getToken()->is_keyword("false"));

  print_success_msg("booleans true and false", 1);
}

void test_define_constant() {
  const auto constant = cast_node<DefineConstantNode>(move(get_element_nodes_from("define PI as double = 3.14")->front()));
  assert(constant->get_type() == Type::DOUBLE);
  assert(constant->get_var_name() == "PI");
  unique_ptr<DoubleNode> value = cast_node<DoubleNode>(constant->retrieve_value_node());
  assert(value->get_token().getStringValue() == "3.14");

  try {
    get_element_nodes_from("define PI as double = store");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("define PI as double");
    assert(false);
  } catch (InvalidSyntaxError e) {}
  
  try {
    get_element_nodes_from("define PI as");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("define PI");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("define as");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  try {
    get_element_nodes_from("define");
    assert(false);
  } catch (InvalidSyntaxError e) {}

  print_success_msg("defines constants", 1);
}

void test_simple_boolean_operators_AND_OR_NOT() {
  const auto and_node = cast_node<AndNode>(move(get_element_nodes_from("true and false")->front()));
  assert(cast_node<BooleanNode>(and_node->retrieve_a())->is_true());
  assert(!cast_node<BooleanNode>(and_node->retrieve_b())->is_true());

  const auto or_node = cast_node<OrNode>(move(get_element_nodes_from("true or false")->front()));
  assert(cast_node<BooleanNode>(or_node->retrieve_a())->is_true());
  assert(!cast_node<BooleanNode>(or_node->retrieve_b())->is_true());

  const auto right_expr = cast_node<OrNode>(move(get_element_nodes_from("true or (store b as int = 5)")->front()));
  assert(cast_node<BooleanNode>(right_expr->retrieve_a())->is_true());
  assert(cast_node<VarAssignmentNode>(right_expr->retrieve_b())->get_var_name() == "b");

  const auto full_expr = cast_node<OrNode>(move(get_element_nodes_from("(store a as int = 0) or (store b as int = 5)")->front()));
  assert(cast_node<VarAssignmentNode>(full_expr->retrieve_a())->get_var_name() == "a");
  assert(cast_node<VarAssignmentNode>(full_expr->retrieve_b())->get_var_name() == "b");

  const auto assigning_boolean_operator = cast_node<VarAssignmentNode>(move(get_element_nodes_from("store a as int = 0 or 5")->front()));
  assert(assigning_boolean_operator->get_var_name() == "a");
  unique_ptr<OrNode> assigned_or_node = cast_node<OrNode>(assigning_boolean_operator->retrieve_value_node());
  assert(cast_node<IntegerNode>(assigned_or_node->retrieve_a())->to_string() == "IntegerNode(0)");
  assert(cast_node<IntegerNode>(assigned_or_node->retrieve_b())->to_string() == "IntegerNode(5)");

  const auto assigning_assignment = cast_node<VarAssignmentNode>(move(get_element_nodes_from("store a as int = (store b as int = 0) or (store c as int = 5)")->front()));
  assert(assigning_assignment->get_var_name() == "a");
  assert(instanceof<OrNode>(assigning_assignment->retrieve_value_node().get()));

  const auto not_node = cast_node<NotNode>(move(get_element_nodes_from("not true")->front()));
  assert(cast_node<BooleanNode>(not_node->retrieve_node())->is_true());

  const auto exclamation_mark_node = cast_node<NotNode>(move(get_element_nodes_from("!true")->front()));
  assert(cast_node<BooleanNode>(exclamation_mark_node->retrieve_node())->is_true());

  const auto exclamation_mark_node_paren = cast_node<NotNode>(move(get_element_nodes_from("!(true)")->front()));
  assert(cast_node<BooleanNode>(exclamation_mark_node_paren->retrieve_node())->is_true());

  // Test invalid syntaxes

  assert(should_throw<InvalidSyntaxError>("!"));
  assert(should_throw<InvalidSyntaxError>("not"));
  assert(should_throw<InvalidSyntaxError>("true or"));
  assert(should_throw<InvalidSyntaxError>("true and"));
  assert(should_throw<InvalidSyntaxError>("true or !"));
  assert(should_throw<InvalidSyntaxError>("not or"));
  assert(should_throw<InvalidSyntaxError>("not not"));
  assert(should_throw<InvalidSyntaxError>("!!"));

  print_success_msg("simple boolean operators (and, or, not)", 1);
}

int main() {
  print_title("Parser tests...");

  try {
    test_simple_number();
    test_simple_decimal_number();
    test_simple_negative_number();
    test_simple_positive_number();
    test_simple_addition();
    test_simple_addition_with_whitespace();
    test_simple_substraction();
    test_simple_substraction_with_whitespace();
    test_simple_multiplication();
    test_simple_divison();
    test_simple_modulo();
    test_simple_power();
    test_complex_maths_expression_with_parenthesis();
    test_complex_maths_expression_without_parenthesis();
    test_mutiple_lines();
    test_positions_on_single_digit();
    test_positions_on_math_expression();
    test_variable_assignment_with_initial_value();
    test_variable_assignment_without_initial_value();
    test_variable_modification();
    test_access_node();
    test_access_node_in_expression();
    test_string();
    test_boolean();
    test_define_constant();
    test_simple_boolean_operators_AND_OR_NOT();
  } catch (CustomError custom_error) {
    cerr << "Oops, the program unexpectedly thrown an error :" << endl;
    cerr << custom_error.get_details() << endl;
    return 1;
  } catch (string cast_exception) {
    cerr << "ABORT. Terminating tests due to this error:" << endl;
    cerr << cast_exception << endl;
    return 1;
  }

  print_success_msg("All \"Parser\" tests successfully passed");
  return 0;
}
