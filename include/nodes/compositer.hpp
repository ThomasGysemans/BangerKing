// This file is the compositer of all nodes.
// Include this header if the goal is to use all types of nodes in a single file.
// It's the case for the parser and the interpreter.

#pragma once

#include "custom_node.hpp"
#include "binary_operation.hpp"
#include "add_node.hpp"
#include "divide_node.hpp"
#include "list_node.hpp"
#include "minus_node.hpp"
#include "modulo_node.hpp"
#include "multiply_node.hpp"
#include "integer_node.hpp"
#include "double_node.hpp"
#include "plus_node.hpp"
#include "power_node.hpp"
#include "substract_node.hpp"
#include "var_assignment_node.hpp"
#include "var_access_node.hpp"
#include "var_modify_node.hpp"
#include "string_node.hpp"
#include "boolean_node.hpp"
#include "define_constant_node.hpp"
#include "and_node.hpp"
#include "or_node.hpp"
#include "not_node.hpp"