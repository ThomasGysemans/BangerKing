#pragma once

#include "nodes/compositer.hpp"
#include "exceptions/invalid_syntax_error.hpp"
#include "token.hpp"

class Parser final {
  /// @brief A pointer to the linked list containing the tokens of the source code.
  const std::list<std::unique_ptr<const Token>>* tokens;

  /// @brief The parser reads the tokens one by one,
  // therefore use this iterator to access the current token during the parsing process.
  std::list<std::unique_ptr<const Token>>::const_iterator iter;

  /// @brief Moves to the next token (by increasing the iterator).
  void advance();

  /// @brief Gets the current token that the parser is reading.
  /// @return The pointer to the current token.
  [[nodiscard]] const std::unique_ptr<const Token>& getTok() const;

  /// @brief Checks if the parser still has some tokens to read.
  /// @return `true` if the parsing isn't completed, `false` otherwise.
  [[nodiscard]] bool has_more_tokens() const;

  /// @brief Is the current token of type `TokenType::NEWLINE`
  /// @return `true` if the token that the Parser is currently reading is of type `NEWLINE`.
  [[nodiscard]] bool is_newline() const;

  /// @brief Skips all the newlines until reaching a different token.
  void ignore_newlines();

  public:
    explicit Parser(const std::list<std::unique_ptr<const Token>>& toks);

    /// @brief Parses the given list of tokens
    /// @return An instance of `ListNode` that contains all the parsed nodes of the code.
    std::unique_ptr<ListNode> parse();

  private:
    /// @brief Reads multiple statements
    /// @return An instance of `ListNode` that contains all the parsed statements
    std::unique_ptr<ListNode> statements();

    /// @brief Reads one single statement on a line.
    std::unique_ptr<CustomNode> statement();

    /// @brief Parses an expression (`expr`), like a variable or a high-level feature
    std::unique_ptr<CustomNode> expr();

    /// @brief Parses a boolean operation (with AND or OR) or any other lower-level feature
    std::unique_ptr<CustomNode> cond_expr();

    /// @brief Parses a comparison operation (with LTE, GTE, GT, LT, etc.)
    std::unique_ptr<CustomNode> comp_expr();

    /// @brief Parses a binary operation that involves binary calculations and an optional equal sign.
    std::unique_ptr<CustomNode> bin_op();

    /// @brief Parses an arithmetic expression
    std::unique_ptr<CustomNode> arith_expr();

    /// @brief Parses the member of an arithmetic expression
    std::unique_ptr<CustomNode> term();

    /// @brief Parses an incrementation, decrementation, or a sign (-5 for example)
    std::unique_ptr<CustomNode> factor();

    /// @brief Parses the call to a property.
    std::unique_ptr<CustomNode> prop();

    /// @brief Parses the property with its actual call (so we expect `[]` or `()`)
    std::unique_ptr<CustomNode> call();

    /// @brief The lowest level feature (a number, a function declaration, a list, an identifier, etc.)
    std::unique_ptr<CustomNode> atom();
};
