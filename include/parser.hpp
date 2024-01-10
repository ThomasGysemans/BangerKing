#pragma once

#include <list>
#include "nodes/compositer.hpp"
#include "exceptions/invalid_syntax_error.hpp"
#include "miscellaneous.hpp"
#include "token.hpp"

using namespace std;

class Parser {
  /// @brief A pointer to the linked list containing the tokens of the source code.
  const list<Token*>* tokens;

  /// @brief The parser reads the tokens one by one,
  // therefore use this iterator to access the current token during the parsing process.
  list<Token*>::const_iterator iter;

  /// @brief Moves to the next token (by increasing the iterator).
  void advance();

  /// @brief Gets the current token that the parser is reading.
  /// @return The pointer to the current token.
  const Token* getTok() const;

  /// @brief Checks if the parser still has some tokens to read.
  /// @return `true` if the parsing isn't completed, `false` otherwise.
  bool has_more_tokens() const;

  /// @brief Is the current token of type `TokenType::NEWLINE`
  /// @return `true` if the token that the Parser is currently reading is of type `NEWLINE`.
  bool is_newline() const;

  /// @brief Skips all the newlines until reaching a different token.
  void ignore_newlines();

  public:
    Parser(const list<Token*>& toks);

    /// @brief Parses the given list of tokens
    /// @return An instance of `ListNode` that contains all the parsed nodes of the code.
    const ListNode* parse();

  private:
    /// @brief Reads multiple statements
    /// @return An instance of `ListNode` that contains all the parsed statements
    const ListNode* statements();

    /// @brief Reads one single statement on a line.
    const CustomNode* statement();

    /// @brief Parses an expression (`expr`), like a variable or a high-level feature
    const CustomNode* expr();

    /// @brief Parses a boolean operation (with AND or OR) or any other lower-level feature
    const CustomNode* cond_expr();

    /// @brief Parses a comparison operation (with LTE, GTE, GT, LT, etc.)
    const CustomNode* comp_expr();

    /// @brief Parses a binary operation that involves binary calculations and an optional equal sign.
    const CustomNode* bin_op();

    /// @brief Parses an arithmetic expression
    const CustomNode* arith_expr();

    /// @brief Parses the member of an arithmetic expression
    const CustomNode* term();

    /// @brief Parses an incrementation, decrementation, or a sign (-5 for example)
    const CustomNode* factor();

    /// @brief Parses the call to a property.
    const CustomNode* prop();

    /// @brief Parses the property with its actual call (so we expect `[]` or `()`)
    const CustomNode* call();

    /// @brief The lowest level feature (a number, a function declaration, a list, an identifier, etc.)
    const CustomNode* atom();
};
