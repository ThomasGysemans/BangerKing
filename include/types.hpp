#pragma once

#include <string>

enum Type {
  INT,
  DOUBLE,
  STRING,
  BOOLEAN,
  LIST
};

/// @brief Gets the name of a built-in type.
/// @param type A built-in reference of `Type`
/// @return The name of the given built-in type.
std::string get_type_name(const Type& type);
