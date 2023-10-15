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
