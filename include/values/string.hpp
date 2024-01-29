#pragma once

#include "value.hpp"

class IntegerValue;

class StringValue: public Value {
  public:
    StringValue(std::string v);
    StringValue();

    /// @brief Gets the actual C++ value that this class contains.
    /// @return The integer that this value holds.
    std::string get_actual_value() const;

    /// @brief Gets the default C++ value that this class should give to variables without initial value.
    /// @return The default value for a string (an empty string).
    static std::string get_default_value();
    
    bool is_truthy() const override;
    std::string to_string() const override;
    StringValue* copy() const override;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the StringValue:
    /// - Type::INT => the length of the string
    std::unique_ptr<Value> cast(Type output_type) const override;

    // Additions (concatenations)
    // The logic will be the same for any type of value,
    // which is just creating a new StringValue
    // with the concatenation of the call to `to_string` from both operands.
    // A concatenation will be possible with any type of value.
    StringValue* operator+(const Value& other) const;

    // Multiplications
    StringValue* operator*(const IntegerValue& other) const; // repeats the string `other` times

    /// @brief Makes concatenation possible when a value, which is not a string, is the left operand of an addition involving a string.
    /// @param left The first operand (any other type of value than a string).
    /// @param right The second operation (necessarily a string).
    /// @return A new StringValue holding the concatenation of the two operands.
    static StringValue* make_concatenation_rtl(const Value* left, const StringValue* right);
};
