#include "../include/parser.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/invalid_syntax_error.hpp"
using namespace std;

/*
*
* Helper methods of parsing
*
*/

bool Parser::has_more_tokens() const {
  return lexer->hasMoreTokens() || current_token != nullptr;
}

bool Parser::is_newline() {
  return get_tok()->ofType(TokenType::NEWLINE);
}

void Parser::ignore_newlines() {
  while (has_more_tokens() && is_newline()) {
    advance();
  }
}

void Parser::require_token(const Position& pos) const {
  if (current_token == nullptr) {
    throw InvalidSyntaxError(
      pos, pos,
      "Unexpected end of parsing"
    );
  }
}

shared_ptr<const Token> Parser::expect(const TokenType type, const Position& pos) {
  advance();
  require_token(pos);
  shared_ptr<const Token> token = get_tok();
  if (token->notOfType(type)) {
    throw InvalidSyntaxError(
      token->getStartingPosition(), token->getEndingPosition(),
      "Unexpected token '" + token->getStringValue() + "'"
    );
  }
  return token;
}

void Parser::advance() {
  current_token = lexer->get_next_token();
}

shared_ptr<const Token> Parser::get_tok() {
  return current_token;
}

/*
*
* Parsing
*
*/

Parser Parser::initCLI(const std::string& input) {
  Parser parser;
  parser.lexer = Lexer::readCLI(input);
  // because current_tok is nullptr right now,
  // and it would create issues (seg faults):
  parser.advance();
  return parser;
}

Parser Parser::initFile(const std::shared_ptr<std::string>& source_code, const std::string& path) {
  Parser parser;
  parser.lexer = Lexer::readFile(source_code, path);
  // so that the Parser starts with the first token,
  // instead of having to advance() right at the beginning
  parser.current_token = parser.lexer->get_next_token();
  return parser;
}

unique_ptr<ListNode> Parser::parse() {
  // To ensure that the unique pointers
  // do not get copied when assigned to a local variable,
  // I do not declare the pointer as `const`.
  // This way, I allow the move semantic to work properly.
  unique_ptr<ListNode> stmts = statements();

  if (has_more_tokens()) {
    const Token invalid_token = get_tok()->copy();
    const Position pos_start = invalid_token.getStartingPosition().copy();
    advance();
    const auto tok = get_tok();
    const Position pos_end = tok != nullptr ? tok->getEndingPosition() : pos_start.copy();
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
  const Position pos_start = get_tok()->getStartingPosition();
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
  if (get_tok()->ofType(TokenType::KEYWORD)) {
    if (get_tok()->is("store")) {
      const Position pos_start = get_tok()->getStartingPosition();
      const string var_name = expect(TokenType::IDENTIFIER, pos_start)->getStringValue();
      advance();
      if (!has_more_tokens()) { // the user wrote "store variable_name"
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected type of variable"
        );
      }
      if (!get_tok()->is_keyword("as")) {
        throw InvalidSyntaxError(
          pos_start, get_tok()->getEndingPosition(),
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
      const Token type_name = get_tok()->copy();
      advance();
      if (has_more_tokens() && get_tok()->ofType(TokenType::EQUALS)) {
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
        // There should not be anything after a variable assignment
        if (has_more_tokens() && !is_newline()) {
          throw InvalidSyntaxError(
            get_tok()->getStartingPosition(), get_tok()->getEndingPosition(),
            "Unexpected token after variable assignment"
          );
        }
        return make_unique<VarAssignmentNode>(
          var_name,
          nullptr,
          type_name,
          pos_start,
          type_name.getEndingPosition()
        );
      }
    } else if (get_tok()->is("define")) {
      const Position pos_start = get_tok()->getStartingPosition();
      const string var_name = expect(TokenType::IDENTIFIER, pos_start)->getStringValue();
      advance();
      if (!has_more_tokens()) {
        throw InvalidSyntaxError(
          pos_start, pos_start,
          "Expected type of variable"
        );
      }
      if (!get_tok()->is_keyword("as")) {
        throw InvalidSyntaxError(
          pos_start, get_tok()->getEndingPosition(),
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
      const Type constant_type = get_type_from_name(get_tok()->getStringValue());
      if (constant_type == Type::ERROR_TYPE) {
        throw InvalidSyntaxError(
          pos_start, get_tok()->getEndingPosition(),
          "Expected a valid native type for this constant"
        );
      }
      advance();
      if (!has_more_tokens() || get_tok()->notOfType(TokenType::EQUALS)) {
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

unique_ptr<CustomNode> Parser::cond_expr() {
  unique_ptr<CustomNode> result = comp_expr();

  while (has_more_tokens() && (get_tok()->is_keyword("and") || get_tok()->is_keyword("or"))) {
    const auto tok = get_tok()->copy();
    const auto tok_pos = tok.getStartingPosition();
    advance();
    if (!has_more_tokens()) {
      throw InvalidSyntaxError(
        result->getStartingPosition(), tok_pos,
        "Unexpected end of boolean expression"
      );
    }
    auto b = comp_expr();
    if (tok.is_keyword("and")) {
      result = make_unique<AndNode>(result, b);
    } else {
      result = make_unique<OrNode>(result, b);
    }
  }

  return result;
}

unique_ptr<CustomNode> Parser::comp_expr() {
  if (get_tok()->ofType(TokenType::NOT) || get_tok()->is_keyword("not")) { // "!" or "not"
    const Position pos_start = get_tok()->getStartingPosition();
    advance();
    if (!has_more_tokens()) {
      throw InvalidSyntaxError(
        pos_start, pos_start,
        "Unexpected end of negation"
      );
    }
    unique_ptr<CustomNode> negation = comp_expr();
    return make_unique<NotNode>(move(negation));
  }

  return bin_op();
}

unique_ptr<CustomNode> Parser::bin_op() { return arith_expr(); }

unique_ptr<CustomNode> Parser::arith_expr() {
  unique_ptr<CustomNode> result = term();

  while (has_more_tokens() && (
    get_tok()->ofType(TokenType::PLUS) ||
    get_tok()->ofType(TokenType::MINUS)
  )) {
    const auto tok = get_tok()->copy();
    const auto tok_pos = tok.getStartingPosition();
    advance();
    if (!has_more_tokens()) {
      throw InvalidSyntaxError(
        result->getStartingPosition(), tok_pos,
        "Unexpected end of arithmetic expression"
      );
    }
    auto ter = term();
    if (tok.ofType(TokenType::PLUS)) {
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
    get_tok()->ofType(TokenType::MULTIPLY) ||
    get_tok()->ofType(TokenType::SLASH) ||
    get_tok()->ofType(TokenType::POWER) ||
    get_tok()->ofType(TokenType::MODULO)
  )) {
    const auto tok = get_tok()->copy();
    const auto tok_pos = tok.getStartingPosition();
    advance();
    if (!has_more_tokens()) {
      throw InvalidSyntaxError(
        result->getStartingPosition(), tok_pos,
        "Unexpected end of term"
      );
    }
    auto fac = factor();
    if (tok.ofType(TokenType::MULTIPLY)) {
      result = make_unique<MultiplyNode>(result, fac);
    } else if (tok.ofType(TokenType::SLASH)) {
      result = make_unique<DivideNode>(result, fac);
    } else if (tok.ofType(TokenType::MODULO)) {
      result = make_unique<ModuloNode>(result, fac);
    } else {
      result = make_unique<PowerNode>(result, fac);
    }
  }

  return result;
}

unique_ptr<CustomNode> Parser::factor() {
  if (get_tok()->ofType(TokenType::PLUS)) { // +5
    advance();
    return make_unique<PlusNode>(factor());
  }
  if (get_tok()->ofType(TokenType::MINUS)) { // -5
    advance();
    return make_unique<MinusNode>(factor());
  }
  return prop();
}

unique_ptr<CustomNode> Parser::prop() { return call(); }

unique_ptr<CustomNode> Parser::call() { return atom(); }

unique_ptr<CustomNode> Parser::atom() {
  const Token first_token = get_tok()->copy();

  if (first_token.ofType(TokenType::LPAREN)) {
    advance();
    ignore_newlines();
    unique_ptr<CustomNode> result = expr();
    ignore_newlines();
    if (get_tok()->notOfType(TokenType::RPAREN)) {
      throw InvalidSyntaxError(
        get_tok()->getStartingPosition(), get_tok()->getEndingPosition(),
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
    const Token var_tok = get_tok()->copy();
    advance();
    if (has_more_tokens() && get_tok()->ofType(TokenType::EQUALS)) {
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
      "Could not parse token '" + first_token.getStringValue() + "'"
    );
  }
}