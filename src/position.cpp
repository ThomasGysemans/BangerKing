#include "../include/position.hpp"
using namespace std;

Position::Position(
  const unsigned int i,
  const unsigned int l,
  const unsigned int c,
  string filename
): idx(i), ln(l), col(c), fn(move(filename)) {}

Position Position::getDefaultPos() {
  return {0, 0, 0, "<hidden>"}; // so as to avoid the return type repetition
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
    col = 0;
  }
}

bool Position::equals(const Position& other) const {
  return this == &other ||
    (other.get_idx() == idx &&
    other.get_col() == col &&
    other.get_ln() == ln &&
    other.get_filename() == fn);
}

string Position::to_string() const {
  return std::to_string(ln) + ":" + std::to_string(col) + ", idx=" + std::to_string(idx);
}

// { *this } is the same as Position(*this)
Position Position::copy() const { return { *this }; }