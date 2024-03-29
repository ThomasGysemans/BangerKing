#pragma once

#include "value.hpp"

class BooleanValue final: public Value {
  public:
    explicit BooleanValue(bool v);
    BooleanValue();

    /// @brief Gets the actual C++ value that this class contains.
    /// @return The bool that this value holds.
    [[nodiscard]] bool get_actual_value() const;

    /// @brief Gets the default C++ value that this class should give to variables without initial value.
    /// @return The default value for a double (false).
    static bool get_default_value();
    
    [[nodiscard]] bool is_truthy() const override;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] BooleanValue* copy() const override;

    /// @brief Transforms this value into another type.
    /// Transforming into the same type will produce an error.
    /// These transformations are possible, from the BooleanValue:
    /// - Type::INT => if true, then 1, if false then 0
    /// - Type::DOUBLE => if true, then 1.0, if false then 0.0
    /// - Type::STRING => "1" or "0"
    [[nodiscard]] std::unique_ptr<Value> cast(Type output_type) const override;
};
