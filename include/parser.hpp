#pragma once

#include "nodes/compositer.hpp"
#include "token.hpp"
#include "lexer.hpp"

class Parser final {
  std::shared_ptr<const Token> current_token = nullptr;

  /// @brief Tells the lexer to keep reading the code until it finds the expected token.
  /// @param type The token that the lexer is supposed to immediately read.
  /// @param pos The position at which we expect a token.
  /// @return The token that the Lexer found.
  /// @throw InvalidSyntaxError if the token that the Lexer read doesn't match the expected one.
  [[nodiscard]] std::shared_ptr<const Token> expect(TokenType type, const Position& pos);

  /// @brief Gets the current token, whatever it might be.
  /// It will return `nullptr` if `advance()` wasn't called first.
  /// @return The current token, or `nullptr` if there is none.
  [[nodiscard]] std::shared_ptr<const Token> get_tok();

  /// @brief Is the current token of type `TokenType::NEWLINE`
  /// @return `true` if the token that the Parser is currently reading is of type `NEWLINE`.
  [[nodiscard]] bool is_newline();

  /// @brief Checks if the parser still has some tokens to read.
  /// @return `true` if the parsing isn't completed, `false` otherwise.
  [[nodiscard]] bool has_more_tokens() const;

  /// @brief Call "has_more_tokens" and if it returns `false` then it throws an error.
  /// @brief pos The position at which a token is required.
  void require_token(const Position& pos) const;

  /// @brief Skips all the newlines until reaching a different token.
  void ignore_newlines();

  /// @brief Reads the next token and store it into `current_token`.
  /// Use `get_tok()` to get this token.
  void advance();

  Parser() = default;

  public:
    // I need it to be public for the tests
    std::unique_ptr<Lexer> lexer = nullptr;

    /// @brief Initializes the lexer so that it reads a line from the CLI.
    /// @param input The line to parse.
    /// @return An instance of Parser.
    static Parser initCLI(const std::string& input);

    /// @brief Initializes the lexer so that it opens a file and starts reading from it.
    /// @param source_code The pointer towards the value from the key-value pair stored in READ_FILES for the given file.
    /// @param path The path towards the file to parse.
    /// @return An instance of Parser.
    static Parser initFile(const std::shared_ptr<std::string>& source_code, const std::string& path);

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
