#pragma once

#include "value.hpp"

class IntegerValue: public Value {
  public:
    IntegerValue(int v);

    /// @brief Gets the actual C++ value that this class contains.
    /// @return The integer that this value holds.
    virtual int get_actual_value() const;
    
    bool is_true() const override;
    string to_string() const override;

    IntegerValue* operator+(const IntegerValue& other) const;
    IntegerValue* operator-(const IntegerValue& other) const;
};
