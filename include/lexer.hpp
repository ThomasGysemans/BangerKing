#pragma once

#include <map>
#include <list>
#include "position.hpp"
#include "token.hpp"
using namespace std;

// The "extern" keyword is used to make sure these static variables aren't only available in this single translation unit.
// They get defined in the implementation file, "lexer.cpp".

/// @brief The digits from 0 to 9.
extern const string NORMAL_DIGITS;

/// @brief The allowed digits (the "normal" digits + the underscore symbol).
extern const string DIGITS;

/// @brief The whole list of accepted ASCII letters for identifiers. Any other letter will not be accepted.
extern const string LETTERS;

/// @brief The whole list of accepted ASCII letters for identifiers + the underscore symbol.
extern const string LETTERS_UNDERSCORE;

/// @brief The whole list of accepted ASCII letters for identifiers + the digits (including the underscore symbol).
extern const string LETTERS_DIGITS;

/// @brief The whole list of escape sequences (\n, \r, \t, etc.).
extern const map<char, char> ESCAPE_CHARACTERS;

/// @brief A double quote ("), allowing internal concatenation.
extern const char DOUBLE_QUOTE;

/// @brief A simple quote ('), which doesn't allow internal concatenation.
extern const char SIMPLE_QUOTE;

/// @brief A simple backslash (\).
extern const char BACKSLASH;

class Lexer {
  /// @brief The source code.
  const string* text;
  
  /// @brief The name of the file holding the source code currently being executed.
  /// Defaults to "<stdin>" if the CLI is used.
  const string filename;

  /// @brief The lexer is reading each character one by one.
  /// This instance of `Position` allows the lexer to know where it is in the source code.
  Position pos;

  /// @brief An iterator will read each character of the source code one by one.
  /// The characters won't get modified, therefore we use a const_iterator.
  string::const_iterator iter;

  /// @brief Moves to the next character in the source code.
  void advance();

  /// @brief Did we not reach the end of the source code?
  /// @return `true` if there is still some code to read
  bool hasMoreTokens() const;

  public:
    /// @brief Creates an instance of lexer.
    /// @param t The source code
    /// @param filename The filename, defaults to <stdin> if the CLI is used.
    Lexer(
      const string* t,
      const string filename = "<stdin>"
    );

    /// @brief Generates the tokens in a list.
    /// @return The list of tokens in the given source code.
    list<unique_ptr<const Token>> generate_tokens();

  private:
    /// @brief Makes an identifier.(a simple word starting with a letter or an underscore)
    /// @return A token that is either a keyword or an identifier.
    unique_ptr<Token> make_identifier();

    /// @brief Makes a number or a DOT.
    /// @return A token for a number (integer as well as float and double), or a dot.
    unique_ptr<Token> make_number();

    /// @brief Makes a token of type + or ++
    /// @return A token of type PLUS or INC
    unique_ptr<Token> make_plus_or_increment();

    /// @brief Makes a token of type - or --
    /// @return A token of type MINUS or DEC
    unique_ptr<Token> make_minus_or_decrement();

    /// @brief Makes a token of type * or **
    /// @return A token of type MUL or POWER
    unique_ptr<Token> make_mul_or_power();

    /// @brief Makes a token of type string.
    /// If double quotes are used, then `allow_concatenation` will be set to `true`.
    /// If simple quotes are used, then `allow_concatenation` will be set to `false`.
    /// @return A token of type STRING.
    unique_ptr<Token> make_string();
};
