#pragma once

/// @brief Holds all the different types of nodes the Parser is able to create.
/// It's important to have all of them statically defined in an en enum,
/// because during the interpretation and the compilation,
/// there is a lot of comparisons of types and dynamic casts,
/// which add up and diminush performance.
/// Each node has its NodeType and the visit
/// methods now use a switch statement on this enum.
namespace NodeType {
    // a namespace is necessary because C++ enums are
    // just variables that are declared in the global scope,
    // and therefore there might be conflicts in the names,
    // in particular with TokenType
    enum Type {
        ADD, // 5 + 5
        AND, // 5 && 5
        BOOLEAN, // true, false
        DEFINE_CONSTANT, // define PI as int = 3.14
        DIVIDE, // 5 / 5
        DOUBLE, // 5.0
        INTEGER, // 5
        LIST, // [5]
        NEGATIVE, // -5
        MODULO, // 5 % 5
        MULTIPLY, // 5 * 5
        NOT, // !true, not true
        OR, // 5 || 5
        POSITIVE, // +5
        POWER, // 5 ** 5
        STRING, // "5"
        SUBSTRACT, // 5 - 5
        VAR_ASSIGNMENT, // store a as int = 5
        VAR_ACCESS, // a
        VAR_MODIFY // a = 5
    };
}