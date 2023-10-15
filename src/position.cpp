#include "../include/position.hpp"

#include <iostream>
using namespace std;

Position::Position(
  const unsigned int i,
  const unsigned int l,
  const unsigned int c,
  const string& filename
): idx(i), ln(l), col(c), fn(filename) {}

Position::~Position() { }

Position Position::getDefaultPos() {
  return Position(0, 0, 0, "<hidden>");
}

string Position::get_filename() const { return fn; }
unsigned int Position::get_ln() const { return ln; }
unsigned int Position::get_col() const { return col; }
unsigned int Position::get_idx() const { return idx; };

void Position::advance(const char& current_char) {
  ++idx;
  ++col;

  if (current_char == '\n') {
    ++ln;
    col = -1;
  }
}

string Position::to_string() const {
  return std::to_string(ln) + ":" + std::to_string(col) + ", idx=" + std::to_string(idx);
}

Position Position::copy() const { return Position(*this); }