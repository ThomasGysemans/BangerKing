#ifndef POSITION_H
#define POSITION_H

#include <iostream>
using namespace std;

class Position {
  unsigned int idx;
  unsigned int ln;
  unsigned int col;
  const string fn;

  public:
    static Position getDefaultPos() {
      return Position(0, 0, 0, "<hidden>");
    }

    // virtual destructor of Position.
    // It's necessary for a base class to hold a virtual method
    // so that `dynamic_cast` can do its magic (used in `instanceof` method).
    virtual ~Position() {}

    string get_filename() const { return fn; }
    unsigned int get_ln() const { return ln; }
    unsigned int get_col() const { return col; }
    unsigned int get_idx() const { return idx; }

    Position(
      const unsigned int i,
      const unsigned int l,
      const unsigned int c,
      const string filename
    ): idx(i), ln(l), col(c), fn(filename) {}

    // Returns the reference to itself
    Position& advance(const char current_char) {
      ++idx;
      ++col;

      if (current_char == '\n') {
        ++ln;
        col = -1;
      }

      return *this;
    }

    Position copy() const {
      return Position(*this);
    }

    string to_string() const {
      return std::to_string(ln) + ":" + std::to_string(col) + ", idx=" + std::to_string(idx);
    }
};

class NullPosition: public Position {
  public:
    NullPosition(): Position(-1, -1, -1, "") {}
};

#endif