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
    /// @brief Gets the default position that an instance can have. It's just 0, 0, 0 and a filename of "<hidden>".
    /// @return An instance of `Position`.
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

    /// @brief Compares two instances of Position.
    /// @param other The instance to compare with.
    /// @return `true` if this instance and the other have the same reference, or they both have the same values.
    bool equals(const Position& other) const;

    /// @brief Increases the position (`idx`) and (`col`).
    /// Takes the current character as argument and if it's a newline
    /// then `col` gets reset to 0 and `ln` gets incremented.
    /// @param current_char 
    void advance(const char& current_char);

    /// @brief Creates a deep copy of this instance.
    /// @return A deep copy.
    Position copy() const;

    string to_string() const;
};
