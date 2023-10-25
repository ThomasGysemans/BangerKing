#pragma once

#include <string>

enum Type {
  INT,
  DOUBLE,
  STRING,
  BOOLEAN,
  LIST,
  ERROR_TYPE // the type that is returned whenever the dev is trying to give an unknown native type to a variable
};

/// @brief Gets the name of a built-in type.
/// @param type A built-in reference of `Type`
/// @return The name of the given built-in type.
std::string get_type_name(const Type& type);

/// @brief Gets the type instance from the name (as string).
/// @param type The name of the desired type.
/// @return The type that corresponds to the given name, ERROR_TYPE if it doesn't exist.
Type get_type_from_name(const std::string& type);