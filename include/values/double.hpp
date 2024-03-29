#pragma once

#include "value.hpp"

// A forward declaration is needed because "double.hpp" also uses "integer.hpp".
class IntegerValue;

class DoubleValue final: public Value {
  public:
    explicit DoubleValue(double v);
    DoubleValue();

    /// @brief Gets the actual C++ value that this class contains.
    /// @return The number that this value holds.
    [[nodiscard]] double get_actual_value() const;

    /// @brief Gets the default C++ value that this class should give to variables without initial value.
    /// @return The default value for a double (0.0).
    static double get_default_value();
    
    [[nodiscard]] bool is_truthy() const override;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] DoubleValue* copy() const override;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the DoubleValue:
    /// - Type::INT => the actual_value but cast into an `int`
    [[nodiscard]] std::unique_ptr<Value> cast(Type output_type) const override;

    DoubleValue* operator+(const DoubleValue& other) const;
    DoubleValue* operator+(const IntegerValue& other) const;

    DoubleValue* operator-(const DoubleValue& other) const;
    DoubleValue* operator-(const IntegerValue& other) const;

    DoubleValue* operator*(const DoubleValue& other) const;
    DoubleValue* operator*(const IntegerValue& other) const;

    DoubleValue* operator/(const DoubleValue& other) const;
    DoubleValue* operator/(const IntegerValue& other) const;
    
    DoubleValue* operator%(const DoubleValue& other) const;
    DoubleValue* operator%(const IntegerValue& other) const;
};
