#include "../include/parser.hpp"

/*
*
* Helper methods of parsing
*
*/

void Parser::advance() {
  ++idx;
  ++iter;
}

Token* Parser::getTok() const { return (*iter); }
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
  list<Token*>& toks
): tokens(&toks) {
  iter = toks.begin();
  idx = 0;
}

const ListNode* Parser::parse() {
  const ListNode * stmts = statements();

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

const ListNode* Parser::statements() {
  const Position pos_start = getTok()->getStartingPosition().copy();
  list<const CustomNode*> stmts;

  ignore_newlines();

  if (!has_more_tokens()) {
    return new ListNode(stmts, pos_start, pos_start);
  }

  do {
    stmts.push_back(statement());
    ignore_newlines();
  } while (has_more_tokens());

  return new ListNode(stmts, pos_start, stmts.back()->getEndingPosition());
}

const CustomNode* Parser::statement() { return expr(); }

const CustomNode* Parser::expr() {
  if (getTok()->is_keyword("store")) {
    const Position pos_start = getTok()->getStartingPosition();
    advance();
    if (!has_more_tokens()) {
      throw InvalidSyntaxError(
        pos_start, pos_start,
        "Expected identifier for variable assignment"
      );
    }
    string var_name = getTok()->getStringValue();
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
    Token type_name = getTok()->copy();
    advance();
    if (has_more_tokens() && getTok()->ofType(TokenType::EQUALS)) {
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          pos_start, type_name.getEndingPosition(),
          "Expected an expression after '=' for variable assignment"
        );
      }
      const CustomNode* value_node = cond_expr();
      return new VarAssignmentNode(
        var_name,
        value_node,
        type_name,
        pos_start
      );
    } else {
      return new VarAssignmentNode(
        var_name,
        nullptr,
        type_name,
        pos_start
      );
    }
  }

  return cond_expr();
}

const CustomNode* Parser::cond_expr() { return comp_expr(); }

const CustomNode* Parser::comp_expr() { return bin_op(); }

const CustomNode* Parser::bin_op() { return arith_expr(); }

const CustomNode* Parser::arith_expr() {
  const CustomNode* result = term();

  while (has_more_tokens() && (
    getTok()->ofType(TokenType::PLUS) ||
    getTok()->ofType(TokenType::MINUS)
  )) {
    if (getTok()->ofType(TokenType::PLUS)) {
      advance();
      result = new AddNode(result, term());
    } else if (getTok()->ofType(TokenType::MINUS)) {
      advance();
      result = new SubstractNode(result, term());
    }
  }

  return result;
}

const CustomNode* Parser::term() {
  const CustomNode * result = factor();

  while (has_more_tokens() && (
    getTok()->ofType(TokenType::MULTIPLY) ||
    getTok()->ofType(TokenType::SLASH) ||
    getTok()->ofType(TokenType::POWER) ||
    getTok()->ofType(TokenType::MODULO)
  )) {
    if (getTok()->ofType(TokenType::MULTIPLY)) {
      advance();
      result = new MultiplyNode(result, factor());
    } else if (getTok()->ofType(TokenType::SLASH)) {
      advance();
      result = new DivideNode(result, factor());
    } else if (getTok()->ofType(TokenType::MODULO)) {
      advance();
      result = new ModuloNode(result, factor());
    } else if (getTok()->ofType(TokenType::POWER)) {
      advance();
      result = new PowerNode(result, factor());
    }
  }

  return result;
}

const CustomNode* Parser::factor() {
  if (getTok()->ofType(TokenType::PLUS)) { // +5
    advance();
    return new PlusNode(factor());
  } else if (getTok()->ofType(TokenType::MINUS)) { // -5
    advance();
    return new MinusNode(factor());
  } else {
    return prop();
  }
}

const CustomNode* Parser::prop() { return call(); }

const CustomNode* Parser::call() { return atom(); }

const CustomNode* Parser::atom() {
  const Token first_token = getTok()->copy();

  if (first_token.ofType(TokenType::LPAREN)) {
    advance();
    ignore_newlines();
    const CustomNode* result = expr();
    ignore_newlines();
    if (getTok()->notOfType(TokenType::RPAREN)) {
      delete result;
      throw InvalidSyntaxError(
        getTok()->getStartingPosition(), getTok()->getEndingPosition(),
        "Expected ')'"
      );
    }
    advance();
    return result;
  } else if (first_token.ofType(TokenType::NUMBER)) {
    advance();
    return new IntegerNode(first_token);
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
      const CustomNode* value_node = expr();
      return new VarModifyNode(var_tok.getStringValue(), value_node, var_tok.getStartingPosition());
    }
    return new VarAccessNode(first_token);
  } else {
    throw InvalidSyntaxError(
      first_token.getStartingPosition(), first_token.getEndingPosition(),
      "Unexpected token " + first_token.getStringValue()
    );
  }
}