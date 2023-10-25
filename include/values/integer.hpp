#pragma once

#include "value.hpp"

class IntegerValue: public Value {
  public:
    IntegerValue(int v);
    IntegerValue();

    /// @brief Gets the actual C++ value that this class contains.
    /// @return The integer that this value holds.
    int get_actual_value() const;

    /// @brief Gets the default C++ value that this class should give to variables without initial value.
    /// @return The default value for an integer (0).
    static int get_default_value();
    
    bool is_true() const override;
    string to_string() const override;

    IntegerValue* operator+(const IntegerValue& other) const;
    IntegerValue* operator-(const IntegerValue& other) const;
    IntegerValue* operator*(const IntegerValue& other) const;
    IntegerValue* operator/(const IntegerValue& other) const;
    IntegerValue* operator%(const IntegerValue& other) const;
};
