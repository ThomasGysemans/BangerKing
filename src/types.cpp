#include "../include/types.hpp"

std::string get_type_name(const Type& type) {
  switch (type) {
    case Type::INT: return "int";
    case Type::DOUBLE: return "double";
    case Type::STRING: return "string";
    case Type::BOOLEAN: return "bool";
    case Type::LIST: return "list";
    default:
      return "Unknown type";
  }
}

Type get_type_from_name(const std::string& type) {
  if (type == "int") return Type::INT;
  if (type == "double") return Type::DOUBLE;
  if (type == "string") return Type::STRING;
  if (type == "bool") return Type::BOOLEAN;
  if (type == "list") return Type::LIST;
  return Type::ERROR_TYPE;
}