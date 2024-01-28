#include "../include/parser.hpp"

/*
*
* Helper methods of parsing
*
*/

void Parser::advance() {
  ++iter;
}

const unique_ptr<const Token>& Parser::getTok() const { return (*iter); }
bool Parser::has_more_tokens() const { return iter != tokens->end(); }
bool Parser::is_newline() const { return getTok()->ofType(TokenType::NEWLINE); }

void Parser::ignore_newlines() {
  while (has_more_tokens() && is_newline()) {
    advance();
  }
}

/*
*
* Parsing
*
*/

Parser::Parser(
  const list<unique_ptr<const Token>>& toks
): tokens(&toks) {
  iter = toks.begin();
}

unique_ptr<ListNode> Parser::parse() {
  // To ensure that the unique pointers
  // do not get copied when assigned to a local variable,
  // I do not declare the pointer as `const`.
  // This way, I allow the move semantic to work properly.
  unique_ptr<ListNode> stmts = statements();

  if (has_more_tokens()) {
    const Token invalid_token = getTok()->copy();
    const Position pos_start = getTok()->getStartingPosition().copy();
    advance();
    const Position pos_end = has_more_tokens() ? getTok()->getEndingPosition() : pos_start.copy();
    throw InvalidSyntaxError(
      pos_start, pos_end,
      "Unexpected end of parsing: unable to parse " + invalid_token.getStringValue()
    );
  }

  return stmts;
}

/*
*
* Parsing of the individual features
*
*/

unique_ptr<ListNode> Parser::statements() {
  const Position pos_start = getTok()->getStartingPosition().copy();
  unique_ptr<list<unique_ptr<CustomNode>>> stmts = make_unique<list<unique_ptr<CustomNode>>>();

  ignore_newlines();

  if (!has_more_tokens()) {
    return make_unique<ListNode>(move(stmts), pos_start, pos_start);
  }

  do {
    unique_ptr<CustomNode> stmt = statement();
    stmts->push_back(move(stmt));
    ignore_newlines();
  } while (has_more_tokens());

  return make_unique<ListNode>(move(stmts), pos_start, stmts->back()->getEndingPosition());
}

unique_ptr<CustomNode> Parser::statement() { return expr(); }

unique_ptr<CustomNode> Parser::expr() {
  if (getTok()->ofType(TokenType::KEYWORD)) {
    if (getTok()->is("store")) {
      const Position pos_start = getTok()->getStartingPosition();
      advance();
      if (!has_more_tokens() || getTok()->notOfType(TokenType::IDENTIFIER)) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected identifier for variable assignment"
        );
      }
      const string var_name = getTok()->getStringValue();
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected type of variable"
        );
      }
      if (!getTok()->is_keyword("as")) {
        throw InvalidSyntaxError(
          pos_start, getTok()->getEndingPosition(),
          "Expected 'as' keyword to declare the type in variable assignment"
        );
      }
      advance();
      const Token type_name = getTok()->copy();
      advance();
      if (has_more_tokens() && getTok()->ofType(TokenType::EQUALS)) {
        advance();
        if (!has_more_tokens()) {
          throw InvalidSyntaxError(
            pos_start, type_name.getEndingPosition(),
            "Expected an expression after '=' for variable assignment"
          );
        }
        unique_ptr<CustomNode> value_node = cond_expr();
        const Position ending_pos = value_node->getEndingPosition();
        return make_unique<VarAssignmentNode>(
          var_name,
          move(value_node),
          type_name,
          pos_start,
          ending_pos
        );
      } else {
        return make_unique<VarAssignmentNode>(
          var_name,
          nullptr,
          type_name,
          pos_start,
          type_name.getEndingPosition()
        );
      }
    } else if (getTok()->is("define")) {
      const Position pos_start = getTok()->getStartingPosition();
      advance();
      if (!has_more_tokens() || getTok()->notOfType(TokenType::IDENTIFIER)) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected identifier for constant assignment"
        );
      }
      const string var_name = getTok()->getStringValue();
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected type of variable"
        );
      }
      if (!getTok()->is_keyword("as")) {
        throw InvalidSyntaxError(
          pos_start, getTok()->getEndingPosition(),
          "Expected 'as' keyword to declare the type in variable assignment"
        );
      }
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected type after 'as' keyword"
        );
      }
      const Type constant_type = get_type_from_name(getTok()->getStringValue());
      if (constant_type == Type::ERROR_TYPE) {
        throw InvalidSyntaxError(
          pos_start, getTok()->getEndingPosition(),
          "Expected a valid native type for this constant"
        );
      }
      advance();
      if (!has_more_tokens() || getTok()->notOfType(TokenType::EQUALS)) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected value for this constant"
        );
      }
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected an expression after '=' for constant assignment"
        );
      }
      unique_ptr<CustomNode> value_node = cond_expr();
      const Position ending_pos = value_node->getEndingPosition();
      return make_unique<DefineConstantNode>(
        var_name,
        move(value_node),
        constant_type,
        pos_start,
        ending_pos
      );
    }
  }

  return cond_expr();
}

unique_ptr<CustomNode> Parser::cond_expr() { return comp_expr(); }

unique_ptr<CustomNode> Parser::comp_expr() { return bin_op(); }

unique_ptr<CustomNode> Parser::bin_op() { return arith_expr(); }

unique_ptr<CustomNode> Parser::arith_expr() {
  unique_ptr<CustomNode> result = term();

  while (has_more_tokens() && (
    getTok()->ofType(TokenType::PLUS) ||
    getTok()->ofType(TokenType::MINUS)
  )) {
    auto& tok = getTok();
    advance();
    auto ter = term();
    if (tok->ofType(TokenType::PLUS)) {
      result = make_unique<AddNode>(result, ter);
    } else {
      result = make_unique<SubstractNode>(result, ter);
    }
  }

  return result;
}

unique_ptr<CustomNode> Parser::term() {
  unique_ptr<CustomNode> result = factor();

  while (has_more_tokens() && (
    getTok()->ofType(TokenType::MULTIPLY) ||
    getTok()->ofType(TokenType::SLASH) ||
    getTok()->ofType(TokenType::POWER) ||
    getTok()->ofType(TokenType::MODULO)
  )) {
    auto& tok = getTok();
    advance();
    auto fac = factor();
    if (tok->ofType(TokenType::MULTIPLY)) {
      result = make_unique<MultiplyNode>(result, fac);
    } else if (tok->ofType(TokenType::SLASH)) {
      result = make_unique<DivideNode>(result, fac);
    } else if (tok->ofType(TokenType::MODULO)) {
      result = make_unique<ModuloNode>(result, fac);
    } else {
      result = make_unique<PowerNode>(result, fac);
    }
  }

  return result;
}

unique_ptr<CustomNode> Parser::factor() {
  if (getTok()->ofType(TokenType::PLUS)) { // +5
    advance();
    return make_unique<PlusNode>(factor());
  } else if (getTok()->ofType(TokenType::MINUS)) { // -5
    advance();
    return make_unique<MinusNode>(factor());
  } else {
    return prop();
  }
}

unique_ptr<CustomNode> Parser::prop() { return call(); }

unique_ptr<CustomNode> Parser::call() { return atom(); }

unique_ptr<CustomNode> Parser::atom() {
  const Token first_token = getTok()->copy();

  if (first_token.ofType(TokenType::LPAREN)) {
    advance();
    ignore_newlines();
    unique_ptr<CustomNode> result = expr();
    ignore_newlines();
    if (getTok()->notOfType(TokenType::RPAREN)) {
      throw InvalidSyntaxError(
        getTok()->getStartingPosition(), getTok()->getEndingPosition(),
        "Expected ')'"
      );
    }
    advance();
    return result;
  } else if (first_token.ofType(TokenType::NUMBER)) {
    advance();
    if (string_contains(first_token.getStringValue(), '.')) {
      return make_unique<DoubleNode>(first_token);
    } else {
      return make_unique<IntegerNode>(first_token);
    }
  } else if (first_token.ofType(TokenType::IDENTIFIER)) {
    const Token var_tok = getTok()->copy();
    advance();
    if (has_more_tokens() && getTok()->ofType(TokenType::EQUALS)) {
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          var_tok.getStartingPosition(), var_tok.getEndingPosition(),
          "Expected a new value to be assigned to the variable."
        );
      }
      unique_ptr<CustomNode> value_node = expr();
      return make_unique<VarModifyNode>(var_tok.getStringValue(), move(value_node), var_tok.getStartingPosition());
    }
    return make_unique<VarAccessNode>(first_token);
  } else if (first_token.ofType(TokenType::STR)) {
    advance();
    return make_unique<StringNode>(first_token);
  } else if (first_token.is_keyword("true") || first_token.is_keyword("false")) {
    advance();
    return make_unique<BooleanNode>(first_token);
  } else {
    throw InvalidSyntaxError(
      first_token.getStartingPosition(), first_token.getEndingPosition(),
      "Unexpected token " + first_token.getStringValue()
    );
  }
}