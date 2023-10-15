#pragma once

#include <string>
using namespace std;

class Position {
  unsigned int idx;
  unsigned int ln;
  unsigned int col;
  // cannot be set as "const" to ensure that the implicit copy assignment operator is created by the compiler.
  // indeed, when assigning an instance of Position to another one, a field cannot be set as "const".
  string fn;

  public:
    static Position getDefaultPos();

    // virtual destructor of Position.
    // It's necessary for a base class to hold a virtual method
    // so that `dynamic_cast` can do its magic (used in `instanceof` method).
    virtual ~Position();

    string get_filename() const;
    unsigned int get_ln() const;
    unsigned int get_col() const;
    unsigned int get_idx() const;

    Position(
      const unsigned int i,
      const unsigned int l,
      const unsigned int c,
      const string& filename
    );

    void advance(const char& current_char);
    Position copy() const;
    string to_string() const;
};