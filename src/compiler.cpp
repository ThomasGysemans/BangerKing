#include "../include/compiler.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/exception.hpp"
#include "../include/exceptions/compiler/compiler_error.hpp"
using namespace std;

// It's required to explicitly define the static attributes
// because without that it creates a linker error during compilation:

ofstream Compiler::out;
array<reg_entry_t, Compiler::REGISTERS_AMOUNT> Compiler::registers;
map<string, storage_t> Compiler::variables;
string Compiler::_start_label;

void Compiler::compile(AST&& ast, const std::string& path) {
  out = ofstream(path);
  if (out.bad()) {
    throw Exception("Compiler error", "Could not open file '" + path + "'");
  }
  _start_label.append(".global _start\n_start:\n");
  if (ast->get_number_of_nodes() != 0) {
    visit_ast(move(ast));
  }
  println("swi 0"); // ends the program
  flush();
  close();
}

void Compiler::flush() {
  out << _start_label;
  if (!variables.empty()) {
    // there is already a \n at the end of the last instruction,
    // so add one just if there is a data section
    out << "\n";
    string all_variables;
    for (const auto& [key, value] : variables) {
      // Exclude the variables that have computed values
      // meaning a variable whose value is the result of an operation:
      // store a as int = 5 + 5 (which is not stored on the stack)
      if (value.value_node != nullptr) {
        all_variables.append("\t" + key + ": " + get_assembly_var_type(value.type) + " " + value.value_node->literal() + "\n");
      }
    }
    if (!all_variables.empty()) {
      out << ".data" << endl;
      out << all_variables;
    }
  }
}

void Compiler::close() {
  out.close();
}

void Compiler::write(const CompilerOperations::VariableAssignment& assignment, const string& new_variable_name, const Type& type, unique_ptr<CustomNode>&& left, unique_ptr<CustomNode>&& right) {
  if (assignment == CompilerOperations::LITERAL) { // store a as int = 5
    init_constant_stack_variable(new_variable_name, type, move(left));
  } else if (CompilerOperations::is_assignment_with_one_literal(assignment)) { // store b as int = a + 6
    // We are operating with a variable (left) and a literal (right).
    // The variable is stored in the stack,
    // so it must be moved to a register (via "ldr"),
    // and then this operation becomes possible:
    // `adds rD, left, #right`
    // Basically here is what's happening (if the access variable isn't already in a register):
    // global _start:
    // _start:
    //   ldr rD, =access_variable
    //   ldr rD, [rD]
    //   <operation> rD, left, #right
    const unique_ptr<VarAccessNode> access_node = cast_node<VarAccessNode>(move(left));
    const storage_t& source = find(access_node->get_var_name(), access_node->getStartingPosition(), access_node->getEndingPosition());
    const unsigned int rD = get_free_register();
    if (source.is_register) { // if the variable (left) is already in the register
      println(get_assembly_operation(assignment) + " r" + std::to_string(rD) + ", r" + std::to_string(source.addr) + ", #" + right->literal());
    } else {
      const unsigned int stack_reg = get_free_register(); // gonna store the stack into this register temporarily
      ldr(stack_reg, access_node->get_var_name(), access_node->getStartingPosition(), access_node->getEndingPosition());
      build_bin_op(assignment, rD, stack_reg, right.get());
      release_register(stack_reg); // don't need it anymore, unlock it
      // Finally, since the variable is moved from the stack to the register,
      // we might as well save this information for future use until it gets replaced.
      variables[access_node->get_var_name()].is_register = true; // the variable is currently in the register, useful to improve performance of future usage until it gets replaced
    }
    // Now that the operation has been done
    // and the result stored in rD,
    // we must push that into the stack:
    push(rD, new_variable_name, type, nullptr);
    assign_register(rD, new_variable_name);
    release_register(rD);
  } else if (CompilerOperations::is_assignment_with_two_literals(assignment)) { // store c as int = 5 + 6
    throw UndefinedBehaviorException("Undefined behavior for variable assignment two literals");
  } else {
    throw UndefinedBehaviorException("Undefined behavior for variable assignment of '" + new_variable_name + "'");
  }
}

void Compiler::println(const string& line) noexcept {
  _start_label.append("\t" + line + "\n");
}

void Compiler::clear(const string& output_file_path) noexcept {
  variables.clear();
  _start_label = "";
  for (unsigned int i = 0; i < REGISTERS_AMOUNT; i++) {
    registers.at(i) = { "", false, false };
  }
  out.close();
  filesystem::resize_file(output_file_path, 0);
  out = ofstream(output_file_path);
}

void Compiler::init_constant_stack_variable(const string& name, const Type& type, unique_ptr<CustomNode>&& literal) {
  require_not_existing_variable(name, literal->getStartingPosition(), literal->getEndingPosition());
  variables.insert(std::make_pair(name, storage_t{ static_cast<unsigned int>(variables.size()), type, move(literal), false }));
}

void Compiler::build_bin_op(const CompilerOperations::VariableAssignment& assignment, const unsigned int rD, const unsigned int rS, const CustomNode* literal) {
  println(get_assembly_operation(assignment) +  " r" + std::to_string(rD) + ", r" + std::to_string(rS) + ", #" + literal->literal());
  assign_temporary_register(rD);
}

void Compiler::ldr(const unsigned int rD, const string& stack_variable_name, const Position& pos_start, const Position& pos_end) {
  require_variable(stack_variable_name, pos_start, pos_end);
  println("ldr r" + std::to_string(rD) + ", =" + stack_variable_name);
  println("ldr r" + std::to_string(rD) + ", [r" + std::to_string(rD) + "]");
  assign_register(rD, stack_variable_name);
}

void Compiler::push(const unsigned int rS, const string& var_name, const Type& type, unique_ptr<CustomNode>&& value_node) {
  println("push {r" + std::to_string(rS) + "}");
  variables.insert(std::make_pair(var_name, storage_t{ rS, type, value_node == nullptr ? nullptr : move(value_node), true }));
}

std::string Compiler::get_assembly_var_type(const Type& type) {
  switch (type) {
    case Type::INT: return ".word";
    case Type::STRING: return ".ascii";
    default:
      throw UndefinedBehaviorException("Unsupported variable type '" + get_type_name(type) + "'");
  }
}

void Compiler::require_not_existing_variable(const string& var_name, const Position& pos_start, const Position& pos_end) {
  try {
    variables.at(var_name);
    throw CompilerError(
    pos_start, pos_end,
    "Undefined symbol '" + var_name + "'"
    );
  } catch (std::out_of_range&) { }
}

void Compiler::require_variable(const string& var_name, const Position& pos_start, const Position& pos_end) {
  try {
    variables.at(var_name);
  } catch (std::out_of_range&) {
    throw CompilerError(
      pos_start, pos_end,
      "Undefined symbol '" + var_name + "'"
    );
  }
}

storage_t& Compiler::find(const string& name, const Position& pos_start, const Position& pos_end) {
  try {
    return variables.at(name);
  } catch (std::out_of_range&) {
    throw CompilerError(
    pos_start, pos_end,
    "Undefined symbol '" + name + "'"
    );
  }
}

bool Compiler::is_register_locked(const unsigned int reg) noexcept {
  return registers[reg].locked;
}

void Compiler::lock_register(const unsigned int reg) noexcept {
  registers[reg].locked = true;
}

void Compiler::release_register(const unsigned int reg) {
  if (!registers[reg].locked) {
    throw Exception("Compiler fatal error", "Tried to release an unlocked register r" + std::to_string(reg));
  }
  registers[reg].locked = false;
}

void Compiler::assign_register(const unsigned int reg, const string& var_name) noexcept {
  registers[reg].name = var_name;
  registers[reg].temporary = false;
}

void Compiler::assign_temporary_register(const unsigned int reg) noexcept {
  registers[reg].name = "";
  registers[reg].temporary = false;
}

unsigned int Compiler::get_free_register() {
  for (unsigned int i = 0; i < REGISTERS_AMOUNT; i++) {
    if (!is_register_locked(i)) {
      lock_register(i);
      return i;
    }
  }
  throw UndefinedBehaviorException("Could not get a free register, all of them are occupied.");
}

bool Compiler::is_literal(const CustomNode* value_node) noexcept {
  const NodeType::Type& type = value_node->getNodeType();
  return
    type == NodeType::INTEGER ||
    type == NodeType::DOUBLE;
}

bool Compiler::is_binary_op(const CustomNode* value_node) noexcept {
  return instanceof<BinaryOperationNode>(value_node);
}

CompilerOperations::VariableAssignment Compiler::get_type_of_bin_op(const BinaryOperationNode *op, const bool literals) {
  switch (op->getNodeType()) {
    case NodeType::ADD: return literals ? CompilerOperations::ADD_LITERALS : CompilerOperations::VAR_ADD_LITERAL;
    case NodeType::SUBSTRACT: return literals ? CompilerOperations::SUB_LITERALS : CompilerOperations::VAR_SUB_LITERAL;
    case NodeType::MULTIPLY: return literals ? CompilerOperations::MUL_LITERALS : CompilerOperations::VAR_MUL_LITERAL;
    default:
      throw UndefinedBehaviorException("Unimplemented binary operation of node '" + op->to_string() + "'");
  }
}

void Compiler::visit_ast(AST&& ast) {
  auto nodes = ast->get_element_nodes();
  for (auto&& statement : *nodes) {
    switch (statement->getNodeType()) {
      case NodeType::VAR_ASSIGNMENT: visit_VarAssignmentNode(cast_node<VarAssignmentNode>(move(statement))); break;
      case NodeType::DEFINE_CONSTANT: visit_DefineConstantNode(cast_node<DefineConstantNode>(move(statement))); break;
      default:
        throw UndefinedBehaviorException("Unimplemented operation for statement '" + statement->to_string() + "'");
    }
  }
}

void Compiler::visit_VarAssignmentNode(std::unique_ptr<VarAssignmentNode>&& node) {
  visit_assignment(node->get_var_name(), node->get_type(), node->retrieve_value_node());
}

void Compiler::visit_DefineConstantNode(std::unique_ptr<DefineConstantNode>&& node) {
  visit_assignment(node->get_var_name(), node->get_type(), node->retrieve_value_node());
}

void Compiler::visit_assignment(const std::string &var_name, const Type &var_type, std::unique_ptr<CustomNode> &&value_node) {
  if (is_literal(value_node.get())) {
    write(CompilerOperations::LITERAL, var_name, var_type, move(value_node));
  } else if (is_binary_op(value_node.get())) {
    const unique_ptr<BinaryOperationNode> bin_op = cast_node<BinaryOperationNode>(move(value_node));
    unique_ptr<CustomNode> left = bin_op->retrieve_a();
    unique_ptr<CustomNode> right = bin_op->retrieve_b();
    const bool is_left_literal = is_literal(left.get());
    const bool is_right_literal = is_literal(right.get());
    if (is_left_literal && is_right_literal) {
      write(get_type_of_bin_op(bin_op.get(), true), var_name, var_type, move(left), move(right));
    } else {
      // A mathematical operation is designed like this:
      // <operation> rD, rS, literal
      // with:
      // - rD: the destination register
      // - rS: the source register
      if (is_left_literal) {
        write(get_type_of_bin_op(bin_op.get(), false), var_name, var_type, move(right), move(left));
      } else if (is_right_literal) {
        write(get_type_of_bin_op(bin_op.get(), false), var_name, var_type, move(left), move(right));
      }
    }
  } else {
    throw UndefinedBehaviorException("Unsupported node for assignment of variable '" + var_name + "'");
  }
}
