#include "../include/lexer.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/illegal_string_error.hpp"
#include "../include/exceptions/illegal_char_error.hpp"
#include "../include/exceptions/unclosed_string_error.hpp"
#include "../include/utils/get_file_size.hpp"
using namespace std;

const string NORMAL_DIGITS = "0123456789";
const string DIGITS = NORMAL_DIGITS + "_";
const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string LETTERS_UNDERSCORE = LETTERS + "_";
const string LETTERS_DIGITS = LETTERS + DIGITS;
const map<char, char> ESCAPE_CHARACTERS{{'n', '\n'}, {'t', '\t'}, {'r', '\r'}};
const char DOUBLE_QUOTE = '"';
const char SIMPLE_QUOTE = '\'';
const char BACKSLASH = '\\';

/// @brief Helper method to know if a specific identifier is a keyword or not.
/// @param keyword The identifier that may or may not be a keyword.
/// @return `true` if the given identifier is indeed a keyword.
bool is_keyword(const string& keyword) {
  return std::find(KEYWORDS.begin(), KEYWORDS.end(), keyword) != KEYWORDS.end();
}

unique_ptr<Lexer> Lexer::readCLI(const string& input) {
  unique_ptr<Lexer> lexer = make_unique<Lexer>();
  lexer->pos = make_unique<Position>(0, 0, 0, "<stdin>");
  lexer->iter = input.begin();
  lexer->input_end = input.end();
  lexer->is_cli = true;
  return lexer;
}

unique_ptr<Lexer> Lexer::readFile(const shared_ptr<string>& source_code, const string& path) {
  unique_ptr<Lexer> lexer = make_unique<Lexer>();
  lexer->pos = make_unique<Position>(0, 0, 0, path);
  lexer->source_code = source_code; // the pointer is shared
  unique_ptr<ifstream> file = make_unique<ifstream>(path);
  if (file->bad()) {
    throw Exception("Fatal", "Could not open file '" + path + "'.");
  } else {
    // the CLI starts at the first character automatically,
    // here we have to set it explicitly:
    lexer->file = move(file);
    lexer->current_character = static_cast<char>(lexer->file->get());
    lexer->file_pos = lexer->file->tellg();
    lexer->file_key = path;
    READ_FILES[path]->reserve(get_file_size(*(lexer->file)));
    READ_FILES[path]->push_back(lexer->current_character);
  }
  return lexer;
}

void Lexer::advance() {
  if (is_cli) {
    ++iter;
    pos->advance(*iter);
  } else {
    file->seekg(file_pos); // going back to the previous position
    current_character = static_cast<char>(file->get()); // get the next character
    file_pos = file->tellg(); // keep track of the position of this character
    if (!file->eof()) { // needs to be done because 0xFF might be added to the end of the string
      READ_FILES[file_key]->push_back(current_character);
    }
    pos->advance(current_character);
  }
}

char Lexer::getChar() const {
  if (is_cli) {
    return (*iter);
  } else {
    return current_character;
  }
}

bool Lexer::hasMoreTokens() const {
  if (is_cli) {
    return iter != input_end;
  } else {
    const bool oef = file->eof();
    if (oef) {
      file->close();
    }
    return !oef;
  }
}

void Lexer::close() const {
  if (!is_cli && file->is_open()) {
    file->close();
  }
}

bool Lexer::is_cli_only() const {
  return is_cli;
}

shared_ptr<const Token> Lexer::get_next_token() {
  while(hasMoreTokens()) {
    if (getChar() == '\n' || getChar() == '\r') {
      const Position pos_start = pos->copy();
      if (getChar() == '\r') {
        advance();
      }
      advance();
      return make_shared<Token>(TokenType::NEWLINE, "\n", pos_start);
    } else if (string_contains(LETTERS_UNDERSCORE, getChar())) { // must be before "make_number()"
      return make_identifier();
    } else if (getChar() == '.' || string_contains(DIGITS, getChar())) { // numbers are allowed to start with a dot (in case they're >= 0 and < 1)
      return make_number();
    } else if (getChar() == '+') {
      return make_plus_or_increment();
    } else if (getChar() == '-') {
      return make_minus_or_decrement();
    } else if (getChar() == '*') {
      return make_mul_or_power();
    } else if (getChar() == '/') {
      const Position pos_start = pos->copy();
      advance();
      return make_shared<Token>(TokenType::SLASH, "/", pos_start, pos.get());
    } else if (getChar() == '%') {
      const Position pos_start = pos->copy();
      advance();
      return make_shared<Token>(TokenType::MODULO, "%", pos_start, pos.get());
    } else if (getChar() == '(') {
      const Position pos_start = pos->copy();
      advance();
      return make_shared<Token>(TokenType::LPAREN, "(", pos_start, pos.get());
    } else if (getChar() == ')') {
      const Position pos_start = pos->copy();
      advance();
      return make_shared<Token>(TokenType::RPAREN, ")", pos_start, pos.get());
    } else if (getChar() == '=') {
      const Position pos_start = pos->copy();
      advance();
      return make_shared<Token>(TokenType::EQUALS, "=", pos_start, pos.get());
    } else if (getChar() == '!') {
      const Position pos_start = pos->copy();
      advance();
      return (make_shared<Token>(TokenType::NOT, "!", pos_start, pos.get())); // TODO: is the string value that necessary
    } else if (getChar() == DOUBLE_QUOTE || getChar() == SIMPLE_QUOTE) {
      return make_string();
    } else {
      if (getChar() == ' ') {
        advance();
      } else {
        throw IllegalCharError(
          *pos, *pos,
          string(1, getChar())
        );
      }
    }
  }
  return nullptr;
}

/*
*
* Makers
* Those methods are responsible of constructing complex types of tokens (such as numbers, identifiers, keywords, etc.)
* They are tokens composed of multiple characters.
*
*/

shared_ptr<Token> Lexer::make_identifier() {
  const Position pos_start = pos->copy();
  string identifier = string(1, getChar());
  advance();

  while (hasMoreTokens() && LETTERS_DIGITS.find(getChar()) != string::npos) {
    identifier += getChar();
    advance();
  }

  const bool keyword = is_keyword(identifier);
  const TokenType token_type = keyword ? TokenType::KEYWORD : TokenType::IDENTIFIER;
  return make_shared<Token>(token_type, identifier, pos_start, pos.get());
}

shared_ptr<Token> Lexer::make_number() {
  const Position pos_start = pos->copy();
  const bool is_beginning_with_dot = getChar() == '.';
  string number_str = string(1, getChar());
  int decimal_point_count = 0;
  advance();

  if (is_beginning_with_dot && !string_contains(NORMAL_DIGITS, getChar())) {
    return make_shared<Token>(TokenType::DOT, ".", pos_start);
  }

  const string digits_and_point = DIGITS + ".";
  while (hasMoreTokens() && string_contains(digits_and_point, getChar())) {
    if (getChar() == '.') {
      ++decimal_point_count;
      if (decimal_point_count > 1) {
        break;
      }
    }

    number_str += getChar();
    advance();
  }

  if (number_str.starts_with('.')) {
    number_str = '0' + number_str;
  } else if (number_str.ends_with('.')) {
    number_str += '0';
  }

  remove_character(number_str, '_');
  return make_shared<Token>(TokenType::NUMBER, number_str, pos_start, pos.get());
}

shared_ptr<Token> Lexer::make_plus_or_increment() {
  const Position pos_start = pos->copy();
  TokenType tok_type = TokenType::PLUS;
  string value = "+";
  advance();

  if (getChar() == '+') {
    advance();
    tok_type = TokenType::INC;
    value = "++";
  }

  return make_shared<Token>(tok_type, value, pos_start, pos.get());
}

shared_ptr<Token> Lexer::make_minus_or_decrement() {
  const Position pos_start = pos->copy();
  TokenType tok_type = TokenType::MINUS;
  string value = "-";
  advance();

  if (getChar() == '-') {
    advance();
    tok_type = TokenType::DEC;
    value = "--";
  }

  return make_shared<Token>(tok_type, value, pos_start, pos.get());
}

shared_ptr<Token> Lexer::make_mul_or_power() {
  const Position pos_start = pos->copy();
  TokenType tok_type = TokenType::MULTIPLY;
  string value = "*";
  advance();

  if (getChar() == '*') {
    advance();
    tok_type = TokenType::POWER;
    value = "**";
  }

  return make_shared<Token>(tok_type, value, pos_start, pos.get());
}

shared_ptr<Token> Lexer::make_string() {
  const Position pos_start = pos->copy();
  const char opening_quote = (getChar());
  bool allow_concatenation = getChar() == DOUBLE_QUOTE;
  string value;
  advance();

  bool escaped = false; // `true` if the previous character was a backslash (\)
  while (
    hasMoreTokens() &&
    (escaped || getChar() != opening_quote)
  ) {
    if (value.length() == UINT_MAX) {
      throw IllegalStringError(
        pos_start, *pos,
        "The maximum length of a string has been reached: " + std::to_string(value.length())
      );
    }
    if (getChar() == BACKSLASH) {
      if (escaped) {
        value.push_back(BACKSLASH);
        escaped = false;
      } else {
        escaped = true;
      }
    } else {
      value.push_back(getChar());
      escaped = false;
    }
    advance();
  }

  // If the program reached the end of the source code,
  // it means it never encountered the ending quote,
  // meaning that the string was never closed.
  if (!hasMoreTokens()) {
    throw UnclosedStringError(
      pos_start, pos_start,
      "Reached the end of the code without closing this string literal"
    );
  }

  advance(); // to skip the ending quote (the lexer must not believe it's the start of a new string).

  return make_shared<Token>(TokenType::STR, value, pos_start, pos.get(), allow_concatenation);
}