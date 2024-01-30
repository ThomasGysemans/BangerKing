#include <iostream>
#include <list>
#include "doctest.h"
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

DOCTEST_TEST_SUITE("Parser") {
  SCENARIO("simple number") {
    const auto element_nodes = get_element_nodes_from("5");
    const auto number_node = cast_node<IntegerNode>(move(element_nodes->front()));
    CHECK(element_nodes->size() == 1);
    CHECK(number_node->get_token().getStringValue() == "5");
  }

  SCENARIO("simple decimal number") {
    const auto element_nodes = get_element_nodes_from("3.14");
    const auto number_node = cast_node<DoubleNode>(move(element_nodes->front()));
    CHECK(element_nodes->size() == 1);
    CHECK(number_node->get_token().getStringValue() == "3.14");
    CHECK(number_node->to_string() == "DoubleNode(3.14)"); // to make sure the double is printed out correctly
  }

  SCENARIO("simple negative number") {
    const auto nodes = get_element_nodes_from("-5");
    const auto minus_node = cast_node<MinusNode>(move(nodes->front()));
    const auto number_node = cast_node<IntegerNode>(minus_node->retrieve_node());
    CHECK(nodes->size() == 1);
    CHECK(number_node->get_token().getStringValue() == "5");
  }

  SCENARIO("simple positive number") {
    const auto nodes = get_element_nodes_from("+670");
    const auto plus_node = cast_node<PlusNode>(move(nodes->front()));
    const auto number_node = cast_node<IntegerNode>(plus_node->retrieve_node());
    CHECK(nodes->size() == 1);
    CHECK(number_node->get_token().getStringValue() == "670");
  }

  SCENARIO("simple addition") {
    const auto nodes = get_element_nodes_from("5+6");
    const auto add_node = cast_node<AddNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(add_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(add_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "5");
    CHECK(b->get_token().getStringValue() == "6");
  }

  SCENARIO("simple addition with whitespace") {
    const auto nodes = get_element_nodes_from("5 + 6");
    const auto add_node = cast_node<AddNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(add_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(add_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "5");
    CHECK(b->get_token().getStringValue() == "6");
  }

  SCENARIO("simple substraction") {
    const auto nodes = get_element_nodes_from("0-6");
    const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(sub_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(sub_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "0");
    CHECK(b->get_token().getStringValue() == "6");
  }

  SCENARIO("simple substraction with whitespace") {
    const auto nodes = get_element_nodes_from("0 - 6");
    const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(sub_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(sub_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "0");
    CHECK(b->get_token().getStringValue() == "6");
  }

  SCENARIO("simple multiplication") {
    const auto nodes = get_element_nodes_from("0*6");
    const auto mul_node = cast_node<MultiplyNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(mul_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(mul_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "0");
    CHECK(b->get_token().getStringValue() == "6");
  }

  SCENARIO("simple divison") {
    const auto nodes = get_element_nodes_from("10/2");
    const auto div_node = cast_node<DivideNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(div_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(div_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "10");
    CHECK(b->get_token().getStringValue() == "2");
  }

  SCENARIO("simple modulo") {
    const auto nodes = get_element_nodes_from("13%12");
    const auto modulo_node = cast_node<ModuloNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(modulo_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(modulo_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "13");
    CHECK(b->get_token().getStringValue() == "12");
  }

  SCENARIO("simple power") {
    const auto nodes = get_element_nodes_from("10**2");
    const auto power_node = cast_node<PowerNode>(move(nodes->front()));
    const auto a = cast_node<IntegerNode>(power_node->retrieve_a());
    const auto b = cast_node<IntegerNode>(power_node->retrieve_b());
    CHECK(nodes->size() == 1);
    CHECK(a->get_token().getStringValue() == "10");
    CHECK(b->get_token().getStringValue() == "2");
  }

  SCENARIO("complex maths expression with parenthesis") {
    const auto nodes = get_element_nodes_from("10+(5-2)/6"); // AddNode ( IntegerNode(10), DivideNode ( SubstractNode, IntegerNode(6) ) )
    CHECK(nodes->size() == 1);

    const auto add_node = cast_node<AddNode>(move(nodes->front()));
    const auto add_a = cast_node<IntegerNode>(add_node->retrieve_a()); // 10
    const auto add_b = cast_node<DivideNode>(add_node->retrieve_b()); // DivideNode( SubstractNode(), IntegerNode(10) )
    CHECK(add_a->get_token().getStringValue() == "10");

    const auto sub_node = cast_node<SubstractNode>(add_b->retrieve_a());
    const auto sub_a = cast_node<IntegerNode>(sub_node->retrieve_a());
    const auto sub_b = cast_node<IntegerNode>(sub_node->retrieve_b());
    CHECK(sub_a->get_token().getStringValue() == "5");
    CHECK(sub_b->get_token().getStringValue() == "2");

    const auto den = cast_node<IntegerNode>(add_b->retrieve_b());
    CHECK(den->get_token().getStringValue() == "6");
  }

  SCENARIO("complex maths expression without parenthesis") {
    const auto nodes = get_element_nodes_from("10+5-2/6"); // SubstractNode( AddNode(10, 5), DivideNode(2, 6) )
    CHECK(nodes->size() == 1);

    const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
    const auto add_node = cast_node<AddNode>(sub_node->retrieve_a());
    const auto div_node = cast_node<DivideNode>(sub_node->retrieve_b());

    const auto add_a = cast_node<IntegerNode>(add_node->retrieve_a());
    const auto add_b = cast_node<IntegerNode>(add_node->retrieve_b());
    CHECK(add_a->get_token().getStringValue() == "10");
    CHECK(add_b->get_token().getStringValue() == "5");

    const auto div_a = cast_node<IntegerNode>(div_node->retrieve_a());
    const auto div_b = cast_node<IntegerNode>(div_node->retrieve_b());
    CHECK(div_a->get_token().getStringValue() == "2");
    CHECK(div_b->get_token().getStringValue() == "6");
  }

  SCENARIO("mutiple lines") {
    const auto nodes = get_element_nodes_from(
      "1+2\n"
      "3-4"
    );
    CHECK(nodes->size() == 2);

    const auto first_node = cast_node<AddNode>(move(nodes->front()));
    const auto second_node = cast_node<SubstractNode>(move(nodes->back()));

    CHECK(cast_node<IntegerNode>(first_node->retrieve_a())->get_token().getStringValue() == "1");
    CHECK(cast_node<IntegerNode>(first_node->retrieve_b())->get_token().getStringValue() == "2");
    CHECK(cast_node<IntegerNode>(second_node->retrieve_a())->get_token().getStringValue() == "3");
    CHECK(cast_node<IntegerNode>(second_node->retrieve_b())->get_token().getStringValue() == "4");
  }

  SCENARIO("positions on single digit") {
    const auto nodes = get_element_nodes_from("5");
    CHECK(nodes->size() == 1);

    const auto node = cast_node<IntegerNode>(move(nodes->front()));
    const auto pos_start = node->getStartingPosition();
    const auto pos_end = node->getEndingPosition();

    CHECK(pos_start.get_idx() == 0);
    CHECK(pos_start.get_col() == 0);
    CHECK(pos_start.get_ln() == 0);

    CHECK(pos_end.get_idx() == 1);
    CHECK(pos_end.get_col() == 1);
    CHECK(pos_end.get_ln() == 0);
  }

  SCENARIO("positions on math expression") {
    const auto nodes = get_element_nodes_from("5+6-2");
    CHECK(nodes->size() == 1);

    const auto sub_node = cast_node<SubstractNode>(move(nodes->front()));
    const auto add_node = cast_node<AddNode>(sub_node->retrieve_a());
    const auto two = cast_node<IntegerNode>(sub_node->retrieve_b());

    const auto five = cast_node<IntegerNode>(add_node->retrieve_a());
    const auto six = cast_node<IntegerNode>(add_node->retrieve_b());

    CHECK(five->get_token().getStringValue() == "5");
    CHECK(six->get_token().getStringValue() == "6");
    CHECK(two->get_token().getStringValue() == "2");

    CHECK(five->getStartingPosition().get_idx() == 0);
    CHECK(five->getEndingPosition().get_idx() == 1);
    CHECK(six->getStartingPosition().get_idx() == 2);
    CHECK(six->getEndingPosition().get_idx() == 3);
    CHECK(two->getStartingPosition().get_idx() == 4);
    CHECK(two->getEndingPosition().get_idx() == 5);

    CHECK(sub_node->getStartingPosition().get_idx() == five->getStartingPosition().get_idx());
    CHECK(sub_node->getEndingPosition().get_idx() == two->getEndingPosition().get_idx());
    CHECK(add_node->getStartingPosition().get_idx() == five->getStartingPosition().get_idx());
    CHECK(add_node->getEndingPosition().get_idx() == six->getEndingPosition().get_idx());
  }

  SCENARIO("variable assignment with initial value") {
    const string code = "store a as int = 5";
    const auto nodes = get_element_nodes_from(code);
    CHECK(nodes->size() == 1);

    const auto node = cast_node<VarAssignmentNode>(move(nodes->front()));
    CHECK(node->get_type_name() == "int");
    CHECK(node->get_var_name() == "a");
    CHECK(node->has_value());

    CHECK(node->getStartingPosition().get_idx() == 0);
    CHECK(node->getEndingPosition().get_idx() == code.size());

    unique_ptr<CustomNode> value_node = node->retrieve_value_node();
    CHECK(value_node->getStartingPosition().get_idx() == code.size() - 1);
    CHECK(value_node->getEndingPosition().get_idx() == code.size());
  }

  SCENARIO("variable assignment without initial value") {
    const string code = "store a as int";
    const auto nodes = get_element_nodes_from(code);
    CHECK(nodes->size() == 1);

    const auto node = cast_node<VarAssignmentNode>(move(nodes->front()));
    CHECK(node->get_type_name() == "int");
    CHECK(node->get_var_name() == "a");
    CHECK(!node->has_value());

    CHECK(node->getStartingPosition().get_idx() == 0);
    CHECK(node->getEndingPosition().get_idx() == code.size());
  }

  SCENARIO("variable assignment invalid syntax") {
    CHECK_THROWS_AS(get_element_nodes_from("store a as int = store"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("store a as int 8"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("store as int"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("store as"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("store a"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("store"), InvalidSyntaxError);
  }

  SCENARIO("variable modification") {
    const string code = "a = 5";
    const auto nodes = get_element_nodes_from(code);
    CHECK(nodes->size() == 1);

    const auto var_modify_node = cast_node<VarModifyNode>(move(nodes->front()));
    CHECK(var_modify_node->get_var_name() == "a");

    const auto integer = cast_node<IntegerNode>(var_modify_node->retrieve_value_node());
    CHECK(integer->get_token().getStringValue() == "5");

    CHECK(var_modify_node->getStartingPosition().get_idx() == 0);
    CHECK(var_modify_node->getEndingPosition().get_idx() == code.size());
    CHECK(integer->getStartingPosition().get_idx() == 4);
    CHECK(integer->getEndingPosition().get_idx() == 5);
  }

  SCENARIO("access node") {
    const string code = "variable";
    const auto nodes = get_element_nodes_from(code);
    CHECK(nodes->size() == 1);

    const auto node = cast_node<VarAccessNode>(move(nodes->front()));
    CHECK(node->get_var_name() == "variable");
    CHECK(node->to_string() == "(variable)");
    CHECK(node->getStartingPosition().get_idx() == 0);
    CHECK(node->getEndingPosition().get_idx() == code.size());
  }

  SCENARIO("access node in expression") {
    const string code = "a+5";
    const auto nodes = get_element_nodes_from(code);
    CHECK(nodes->size() == 1);

    const auto node = cast_node<AddNode>(move(nodes->front()));
    CHECK(instanceof<VarAccessNode>(node->retrieve_a().get()));
    CHECK(instanceof<IntegerNode>(node->retrieve_b().get()));
    CHECK(node->getStartingPosition().get_idx() == 0);
    CHECK(node->getEndingPosition().get_idx() == code.size());
  }

  SCENARIO("string") {
    const auto double_quote_string = cast_node<StringNode>(move(get_element_nodes_from("\"hello\"")->front()));
    CHECK(double_quote_string->canConcatenate());
    CHECK(double_quote_string->getToken()->canConcatenate());
    CHECK(double_quote_string->getToken()->ofType(TokenType::STR));
    CHECK(double_quote_string->getValue() == "hello");

    const auto simple_quote_string = cast_node<StringNode>(move(get_element_nodes_from("'YOYO'")->front()));
    CHECK(!simple_quote_string->canConcatenate());
    CHECK(!simple_quote_string->getToken()->canConcatenate());
    CHECK(simple_quote_string->getToken()->ofType(TokenType::STR));
    CHECK(simple_quote_string->getValue() == "YOYO");

    const auto escaped_backslash = cast_node<StringNode>(move(get_element_nodes_from("'C\\'EST'")->front()));
    CHECK(!escaped_backslash->canConcatenate());
    CHECK(!escaped_backslash->getToken()->canConcatenate());
    CHECK(escaped_backslash->getToken()->ofType(TokenType::STR));
    CHECK(escaped_backslash->getValue() == "C'EST");
  }

  SCENARIO("boolean") {
    const auto t = cast_node<BooleanNode>(move(get_element_nodes_from("true")->front()));
    const auto f = cast_node<BooleanNode>(move(get_element_nodes_from("false")->front()));
    
    CHECK(!t->getToken()->canConcatenate());
    CHECK(t->getToken()->ofType(TokenType::KEYWORD));
    CHECK(t->getToken()->is_keyword("true"));

    CHECK(!f->getToken()->canConcatenate());
    CHECK(f->getToken()->ofType(TokenType::KEYWORD));
    CHECK(f->getToken()->is_keyword("false"));
  }

  SCENARIO("define constant") {
    const auto constant = cast_node<DefineConstantNode>(move(get_element_nodes_from("define PI as double = 3.14")->front()));
    CHECK(constant->get_type() == Type::DOUBLE);
    CHECK(constant->get_var_name() == "PI");
    unique_ptr<DoubleNode> value = cast_node<DoubleNode>(constant->retrieve_value_node());
    CHECK(value->get_token().getStringValue() == "3.14");

    CHECK_THROWS_AS(get_element_nodes_from("define PI as double = store"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("define PI as double"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("define PI as"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("define PI"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("define as"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("define"), InvalidSyntaxError);
  }

  SCENARIO("simple boolean operators AND OR NOT") {
    const auto and_node = cast_node<AndNode>(move(get_element_nodes_from("true and false")->front()));
    CHECK(cast_node<BooleanNode>(and_node->retrieve_a())->is_true());
    CHECK(!cast_node<BooleanNode>(and_node->retrieve_b())->is_true());

    const auto or_node = cast_node<OrNode>(move(get_element_nodes_from("true or false")->front()));
    CHECK(cast_node<BooleanNode>(or_node->retrieve_a())->is_true());
    CHECK(!cast_node<BooleanNode>(or_node->retrieve_b())->is_true());

    const auto right_expr = cast_node<OrNode>(move(get_element_nodes_from("true or (store b as int = 5)")->front()));
    CHECK(cast_node<BooleanNode>(right_expr->retrieve_a())->is_true());
    CHECK(cast_node<VarAssignmentNode>(right_expr->retrieve_b())->get_var_name() == "b");

    const auto full_expr = cast_node<OrNode>(move(get_element_nodes_from("(store a as int = 0) or (store b as int = 5)")->front()));
    CHECK(cast_node<VarAssignmentNode>(full_expr->retrieve_a())->get_var_name() == "a");
    CHECK(cast_node<VarAssignmentNode>(full_expr->retrieve_b())->get_var_name() == "b");

    const auto assigning_boolean_operator = cast_node<VarAssignmentNode>(move(get_element_nodes_from("store a as int = 0 or 5")->front()));
    CHECK(assigning_boolean_operator->get_var_name() == "a");
    unique_ptr<OrNode> assigned_or_node = cast_node<OrNode>(assigning_boolean_operator->retrieve_value_node());
    CHECK(cast_node<IntegerNode>(assigned_or_node->retrieve_a())->to_string() == "IntegerNode(0)");
    CHECK(cast_node<IntegerNode>(assigned_or_node->retrieve_b())->to_string() == "IntegerNode(5)");

    const auto assigning_assignment = cast_node<VarAssignmentNode>(move(get_element_nodes_from("store a as int = (store b as int = 0) or (store c as int = 5)")->front()));
    CHECK(assigning_assignment->get_var_name() == "a");
    CHECK(instanceof<OrNode>(assigning_assignment->retrieve_value_node().get()));

    const auto not_node = cast_node<NotNode>(move(get_element_nodes_from("not true")->front()));
    CHECK(cast_node<BooleanNode>(not_node->retrieve_node())->is_true());

    const auto exclamation_mark_node = cast_node<NotNode>(move(get_element_nodes_from("!true")->front()));
    CHECK(cast_node<BooleanNode>(exclamation_mark_node->retrieve_node())->is_true());

    const auto exclamation_mark_node_paren = cast_node<NotNode>(move(get_element_nodes_from("!(true)")->front()));
    CHECK(cast_node<BooleanNode>(exclamation_mark_node_paren->retrieve_node())->is_true());

    // Test invalid syntaxes

    CHECK_THROWS_AS(get_element_nodes_from("!"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("not"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("true or"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("true and"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("true or !"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("not or"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("not not"), InvalidSyntaxError);
    CHECK_THROWS_AS(get_element_nodes_from("!!"), InvalidSyntaxError);
  }
}
