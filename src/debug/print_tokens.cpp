#include "../../include/debug/print_tokens.hpp"
using namespace std;

string get_token_name(TokenType type) {
  switch (type) {
    case TokenType::NUMBER: return "NUMBER";
    case TokenType::PLUS: return "PLUS";
    case TokenType::MINUS: return "MINUS";
    case TokenType::MULTIPLY: return "MULTIPLY";
    case TokenType::SLASH: return "SLASH";
    case TokenType::POWER: return "POWER";
    case TokenType::MODULO: return "MODULO";
    case TokenType::LPAREN: return "LPAREN";
    case TokenType::RPAREN: return "RPAREN";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::KEYWORD: return "KEYWORD";
    case TokenType::EQUALS: return "EQUALS";
    case TokenType::DOUBLE_EQUALS: return "DOUBLE_EQUALS";
    case TokenType::NOT_EQUAL: return "NOT_EQUAL";
    case TokenType::LT: return "LT";
    case TokenType::GT: return "GT";
    case TokenType::LTE: return "LTE";
    case TokenType::GTE: return "GTE";
    case TokenType::NOT: return "!";
    case TokenType::COLON: return "COLON";
    case TokenType::NEWLINE: return "NEWLINE";
    case TokenType::LSQUARE: return "LSQUARE";
    case TokenType::RSQUARE: return "RSQUARE";
    case TokenType::LBRACK: return "LBRACK";
    case TokenType::RBRACK: return "RBRACK";
    case TokenType::COMMA: return "COMMA";
    case TokenType::STR: return "STRING";
    case TokenType::DOT: return "DOT";
    case TokenType::HASH: return "HASH";
    case TokenType::INC: return "INC";
    case TokenType::DEC: return "DEC";
    default:
      return "UNKNOWN_TOKEN(" + std::to_string(type) + ")";
  }
}

string display_tokens_list(const list<unique_ptr<const Token>>& l) {
  auto iter = l.begin();
  string result = "Tokens(" + std::to_string(l.size()) + ") : ";
  result += "[" + get_token_name((*iter)->getType()) + ":" + (*iter)->getStringValue();
  ++iter;
  while (iter != l.end()) {
    result += ", " + get_token_name((*iter)->getType()) + ":" + (*iter)->getStringValue();
    ++iter;
  }
  return result + "]";
}

string display_tokens_list(const list<TokenType>& l) {
  auto iter = l.begin();
  string result = "Tokens(" + std::to_string(l.size()) + ") : ";
  result += "[" + get_token_name(*iter);
  ++iter;
  while (iter != l.end()) {
    result += ", " + get_token_name(*iter);
    ++iter;
  }
  return result + "]";
}