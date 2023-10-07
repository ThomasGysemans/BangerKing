#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <list>
#include "token.h"
#include "nodes.h"
#include "miscellaneous.h"
#include "exceptions/invalid_syntax_error.h"

using namespace std;

class Parser {
  list<Token*>* tokens;
  list<Token*>::const_iterator iter;
  unsigned int idx;

  void advance() {
    ++idx;
    ++iter;
  }

  Token* getTok() const {
    return (*iter);
  }

  bool has_more_tokens() const {
    return iter != tokens->end();
  }

  bool is_newline() const {
    return getTok()->ofType(TokenType::NEWLINE);
  }

  void ignore_newlines() {
    while (has_more_tokens() && is_newline()) {
      advance();
    }
  }

  public:
    Parser(list<Token*>& toks): tokens(&toks) {
      iter = toks.begin();
      idx = 0;
    }

    const ListNode * parse() {
      // list<const CustomNode*> stmts;
      // stmts.push_back(statement());

      // const ListNode * list_node = new ListNode(stmts);

      // return list_node;

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

  private:
    const ListNode * statements() {
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

    const CustomNode * statement() {
      // some more complex stuff

      return expr();
    }

    const CustomNode * expr() {
      // some more stuff

      return cond_expr();
    }

    const CustomNode * cond_expr() {
      const CustomNode * result = comp_expr();
      
      return result;
    }

    const CustomNode * comp_expr() {
      const CustomNode * result = bin_op();

      return result;
    }

    const CustomNode * bin_op() {
      const CustomNode * result = arith_expr();

      return result;
    }

    const CustomNode * arith_expr() {
      const CustomNode * result = term();

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

    const CustomNode * term() {
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

    const CustomNode * factor() {
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

    const CustomNode * prop() {
      const CustomNode * node_to_call = call();

      // there is no property and methods yet

      return node_to_call;
    }

    const CustomNode * call() {
      const CustomNode* atm = atom();

      // it's not possible to call a function or an array yet...

      return atm;
    }

    const CustomNode * atom() {
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
        return new NumberNode(first_token);
      } else {
        throw InvalidSyntaxError(
          first_token.getStartingPosition(), first_token.getEndingPosition(),
          "Unexpected token " + first_token.getStringValue()
        );
      }
    }
};

#endif