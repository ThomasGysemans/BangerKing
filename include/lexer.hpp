#pragma once

#include <map>
#include <list>
#include "position.hpp"
#include "token.hpp"
using namespace std;

extern const string NORMAL_DIGITS;
extern const string DIGITS;
extern const string LETTERS;
extern const string LETTERS_UNDERSCORE;
extern const string LETTERS_DIGITS;
extern const string LETTERS_DIGITS_UNDERSCORE;
extern const map<char, char> ESCAPE_CHARACTERS;

class Lexer {
  const string* text;
  const string filename;
  Position pos;
  string::const_iterator iter;

  /// @brief Moves to the next character in the source code.
  void advance();

  /// @brief Did we reach the end of the source code?
  /// @return `true` if there is still some code to read
  bool hasMoreTokens() const;

  public:
    /// @brief Creates an instance of lexer.
    /// @param t The source code
    /// @param filename The filename
    Lexer(
      const string* t,
      const string filename = "<stdin>"
    );

    /// @brief Generates the tokens in a list.
    /// @return The list of tokens in the given source code.
    list<Token*> generate_tokens();

  private:
    /// @brief Makes an identifier.(a simple word starting with a letter or an underscore)
    /// @return A token that is either a keyword or an identifier.
    Token * make_identifier();

    /// @brief Makes a number or a DOT.
    /// @return A token for a number (integer as well as float and double), or a dot.
    Token * make_number();

    /// @brief Makes a token of type + or ++
    /// @return A token of type PLUS or INC
    Token * make_plus_or_increment();

    /// @brief Makes a token of type - or --
    /// @return A token of type MINUS or DEC
    Token * make_minus_or_decrement();

    /// @brief Makes a token of type * or **
    /// @return A token of type MUL or POWER
    Token * make_mul_or_power();
};
