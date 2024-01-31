#pragma once

#include <string>

class Position final {
  unsigned int idx;
  unsigned int ln;
  unsigned int col;
  // cannot be set as "const" to ensure that the implicit copy assignment operator is created by the compiler.
  // indeed, when assigning an instance of Position to another one, a field cannot be set as "const".
  std::string fn;

  public:
    /// @brief Gets the default position that an instance can have. It's just 0, 0, 0 and a filename of "<hidden>".
    /// @return An instance of `Position`.
    static Position getDefaultPos();

    [[nodiscard]] std::string get_filename() const;
    [[nodiscard]] unsigned int get_ln() const;
    [[nodiscard]] unsigned int get_col() const;
    [[nodiscard]] unsigned int get_idx() const;

    Position(
      unsigned int i,
      unsigned int l,
      unsigned int c,
      std::string filename
    );

    /// @brief Compares two instances of Position.
    /// @param other The instance to compare with.
    /// @return `true` if this instance and the other have the same reference, or they both have the same values.
    [[nodiscard]] bool equals(const Position& other) const;

    /// @brief Increases the position (`idx`) and (`col`).
    /// Takes the current character as argument and if it's a newline
    /// then `col` gets reset to 0 and `ln` gets incremented.
    /// @param current_char 
    void advance(const char& current_char);

    /// @brief Creates a deep copy of this instance.
    /// @return A deep copy.
    [[nodiscard]] Position copy() const;

    [[nodiscard]] std::string to_string() const;
};
