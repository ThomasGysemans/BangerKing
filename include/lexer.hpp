#pragma once

#include <map>
#include <fstream>
#include "position.hpp"
#include "token.hpp"

// The "extern" keyword is used to make sure these static variables aren't only available in this single translation unit.
// They get defined in the implementation file, "lexer.cpp".

/// @brief The digits from 0 to 9.
extern const std::string NORMAL_DIGITS;

/// @brief The allowed digits (the "normal" digits + the underscore symbol).
extern const std::string DIGITS;

/// @brief The whole list of accepted ASCII letters for identifiers. Any other letter will not be accepted.
extern const std::string LETTERS;

/// @brief The whole list of accepted ASCII letters for identifiers + the underscore symbol.
extern const std::string LETTERS_UNDERSCORE;

/// @brief The whole list of accepted ASCII letters for identifiers + the digits (including the underscore symbol).
extern const std::string LETTERS_DIGITS;

/// @brief The whole list of escape sequences (\n, \r, \t, etc.).
extern const std::map<char, char> ESCAPE_CHARACTERS;

/// @brief A double quote ("), allowing internal concatenation.
extern const char DOUBLE_QUOTE;

/// @brief A simple quote ('), which doesn't allow internal concatenation.
extern const char SIMPLE_QUOTE;

/// @brief A simple backslash (\).
extern const char BACKSLASH;

// met leur référence dans le header pour que tous les fichiers y aient accès (sinon ils renvoient "undefined symbol")
// et tu les implémentes (tu donnes une valeur) dans le fichier d'mplementation (lexer.cpp)

class Lexer final {
  bool is_cli = false;

  /// @brief The lexer is reading each character one by one.
  /// This instance of `Position` allows the lexer to know where it is in the source code.
  std::unique_ptr<Position> pos = nullptr;

  // IF WE ARE READING A LINE FROM THE CLI:

  /// @brief An iterator will read each character of the source code one by one.
  /// The characters won't get modified, therefore we use a const_iterator.
  std::string::const_iterator iter;

  /// @brief If we're using the CLI, then it's the end of the input string.
  std::string::const_iterator input_end;

  // IF WE ARE READING A FILE:

  /// @brief The previous character read from the file.
  char current_character = '\0';

  /// @brief The file that the Parser opened.
  std::unique_ptr<std::ifstream> file;

  /// @brief The source code that the Lexer is progressively reading from the file (only if `is_cli` is `false`).
  std::shared_ptr<std::string> source_code;

  /// @brief The path to the file currently being read.
  /// It's the key of the source code stored in READ_FILES.
  std::string file_key;

  /// @brief The position of the current character in this file.
  std::streampos file_pos;

  /// @brief Moves to the next character in the source code.
  void advance();

  /// @brief Gets the current character that the lexer is reading.
  [[nodiscard]] char getChar() const;

  public:
    Lexer() = default;

    /// @brief Creates an instance of Lexer for a single line to analyze.
    /// @param input The single line to analyze from the CLI.
    static std::unique_ptr<Lexer> readCLI(const std::string& input);

    /// @brief Initializes the Lexer for the analysis of a file.
    /// @param source_code The shared pointer towards the source code that the Lexer is going to progressively build up in READ_FILES.
    /// @param path The path towards the file currently being executed. It's a key in READ_FILES.
    static std::unique_ptr<Lexer> readFile(const std::shared_ptr<std::string>& source_code, const std::string& path);

    /// @brief Generates the tokens in a list.
    /// @return The list of tokens in the given source code.
    std::shared_ptr<const Token> get_next_token();

    /// @brief Closes the file, if the lexer is reading one.
    /// If the CLI is used, or if the file is already closed,
    /// then this method does nothing.
    void close() const;

    /// @brief Did we not reach the end of the source code?
    /// @return `true` if there is still some code to read
    [[nodiscard]] bool hasMoreTokens() const;

    [[nodiscard]] bool is_cli_only() const;

  private:
    /// @brief Makes an identifier.(a simple word starting with a letter or an underscore)
    /// @return A token that is either a keyword or an identifier.
    std::shared_ptr<Token> make_identifier();

    /// @brief Makes a number or a DOT.
    /// @return A token for a number (integer as well as float and double), or a dot.
    std::shared_ptr<Token> make_number();

    /// @brief Makes a token of type + or ++
    /// @return A token of type PLUS or INC
    std::shared_ptr<Token> make_plus_or_increment();

    /// @brief Makes a token of type - or --
    /// @return A token of type MINUS or DEC
    std::shared_ptr<Token> make_minus_or_decrement();

    /// @brief Makes a token of type * or **
    /// @return A token of type MUL or POWER
    std::shared_ptr<Token> make_mul_or_power();

    /// @brief Makes a token of type string.
    /// If double quotes are used, then `allow_concatenation` will be set to `true`.
    /// If simple quotes are used, then `allow_concatenation` will be set to `false`.
    /// @return A token of type STRING.
    std::shared_ptr<Token> make_string();
};
