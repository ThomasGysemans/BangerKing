#pragma once

#include "value.hpp"

// A forward declaration is needed because "integer.hpp" also uses "double.hpp".
class DoubleValue;

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
    
    bool is_truthy() const override;
    string to_string() const override;
    IntegerValue* copy() const override;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the IntegerValue:
    /// - Type::DOUBLE => the actual_value but cast into a `double`
    Value* cast(Type output_type) const override;

    // Additions
    IntegerValue* operator+(const IntegerValue& other) const;
    DoubleValue*  operator+(const DoubleValue& other) const;

    // Substractions
    IntegerValue* operator-(const IntegerValue& other) const;
    DoubleValue*  operator-(const DoubleValue& other) const;

    // Multiplications
    IntegerValue* operator*(const IntegerValue& other) const;
    DoubleValue*  operator*(const DoubleValue& other) const;

    // Divisions
    IntegerValue* operator/(const IntegerValue& other) const;
    DoubleValue*  operator/(const DoubleValue& other) const;

    // Modulos
    IntegerValue* operator%(const IntegerValue& other) const;
    DoubleValue*  operator%(const DoubleValue& other) const;
};
