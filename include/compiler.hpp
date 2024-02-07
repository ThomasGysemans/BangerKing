#pragma once

#include <fstream>
#include <array>
#include <map>
#include "nodes/compositer.hpp"
#include "../include/exceptions/undefined_behavior.hpp"

using AST = std::unique_ptr<ListNode>;

/// @brief Describes the operations I might need to write to the output file of the compiler.
/// It also provides helper functions.
namespace CompilerOperations {
  /// @brief There are multiple ways to assign a value to a register:
  /// - a simple literal (store a as int = 5, directly declared onto the stack).
  /// - a binary operation between literals and variables (store b as int = a - 5, leading to a SUBS operation but with particular arguments, then stored onto the stack)
  /// - a binary operation between literals (store a as int = 5 + 6, leading multiple operations, because doing maths between two literals isn't allowed).
  enum VariableAssignment {
    LITERAL,
    VAR_ADD_LITERAL, // any addition including a variable access node and a literal
    VAR_SUB_LITERAL, // any subtraction including a variable acess node and a literal
    VAR_MUL_LITERAL, // any multiplication including a variable access node and a literal
    ADD_LITERALS, // any addition including two literals
    SUB_LITERALS, // any subtraction including two literals
    MUL_LITERALS, // any multiplication including two literals
  };

  enum Basic {
    TERMINATION // swi 0
  };

  static bool is_assignment_with_one_literal(const VariableAssignment& assignment) noexcept {
    return
      assignment == VAR_ADD_LITERAL ||
      assignment == VAR_SUB_LITERAL ||
      assignment == VAR_MUL_LITERAL;
  }

  static bool is_assignment_with_two_literals(const VariableAssignment& assignment) noexcept {
    return
      assignment == ADD_LITERALS ||
      assignment == SUB_LITERALS ||
      assignment == MUL_LITERALS;
  }

  /// @brief Gets the Assembly operation that's fit for the given variable assignment's value node.
  /// - Addition => ADDS
  /// - Subtraction => SUBS
  /// - Multiplication => MUL
  static std::string get_assembly_operation(const VariableAssignment& assignment) {
    switch (assignment) {
      case ADD_LITERALS:
      case VAR_ADD_LITERAL:
        return "adds";
      case SUB_LITERALS:
      case VAR_SUB_LITERAL:
        return "subs";
      case MUL_LITERALS:
      case VAR_MUL_LITERAL:
        return "mul";
      default:
        throw UndefinedBehaviorException("Unsupported operation for variable assignment");
    }
  }
}

/// @brief Defines where a variable is stored.
struct storage_t {
  /// @brief The index in the registers/stack
  unsigned int addr;

  /// @brief The type of the variable (INT, STRING, DOUBLE, etc.)
  Type type;

  /// @brief The value of this variable.
  /// The variable might have a value that has to be computed
  /// and stored in the stack, so this might be `nullptr`.
  std::unique_ptr<CustomNode> value_node;

  /// @brief Is it currently being stored in the register?
  bool is_register;
};

/// @brief Defines how a register behaves.
struct reg_entry_t {
  /// @brief The name of the variable currently being stored.
  /// It might not be storing the value of a variable,
  /// but the result of an operation for example,
  /// so this might be an empty string.
  /// Look at "temporary".
  std::string name;

  /// @brief If this register doesn't hold a variable,
  /// then it must be storing the result of an operation,
  /// which is temporary, and doesn't have a name.
  bool temporary;

  /// @brief Is it locked by an operation currently being written?
  bool locked;
};

class Compiler final {
  static constexpr unsigned int REGISTERS_AMOUNT = 13;

  /// @brief The assembly file currently being written by the compiler.
  static std::ofstream out;

  /// @brief The 13 registers of 32 bits each.
  /// When a variable (not a constant) is accessed,
  /// I replace that name with the register where it's actually stored.
  /// What's difficult here is to manage potentially infinite variables in 13 registers.
  static std::array<reg_entry_t, REGISTERS_AMOUNT> registers;

  /// @brief Stores in a map where the variables are stored.
  /// It stores all variables (so they might also be in "registers").
  static std::map<std::string, storage_t> variables;

  /// @brief The _start label to be added to the file later.
  /// To write it to the file, call `flush()`.
  static std::string _start_label;

  Compiler() = default;

  public:
    /// @brief Compiles an abstract syntax tree produced by the Parser.
    /// It creates a file located at the given path.
    /// It overwrites the file if it already exists.
    /// @param ast The result of the Parser.
    /// @param path The path of the compiled result.
    static void compile(AST&& ast, const std::string& path);

    /// @brief Stops writing to the file.
    static void close();

  // private: // (but too annoying for the tests)

    /// @brief Writes everything that's been compiled into the file.
    /// It creates the .data section from the `variables` array.
    /// It prints `_start_label` into the file, below the .data section.
    /// It doesn't clear anything, the variables remain the same
    /// and the _start_label doesn't get reset.
    static void flush();

    /// @brief Writes a variable assignment to the output file of the compiler (in the "data" section).
    /// @param assignment The type of the operation (produced by the visit to a VariableAssignment custom node).
    /// @param new_variable_name The name of the variable to be stored.
    /// @param type The type of this variable.
    /// @param left The left operand of the assignment in the case of a binary operation node, or the whole value to be stored if it's just a literal.
    /// @param right The optional right operand of the assignment in the case of a binary operation node.
    static void write(const CompilerOperations::VariableAssignment& assignment, const std::string& new_variable_name, const Type& type, std::unique_ptr<CustomNode>&& left, std::unique_ptr<CustomNode>&& right = nullptr);

    /// @brief Writes a simple pre-defined operation in the "_start" label.
    static void write(const CompilerOperations::Basic&);

    /// @brief Prepares a new line to be inserted into the _start label.
    /// The line is indented by one by default.
    /// @param line The line to be written.
    static void println(const std::string& line) noexcept;

    /// @brief Stores a variable directly into the .data section.
    /// It is either a constant (declared with the "define" keyword)
    /// or a variable whose value is a simple constant literal.
    /// In those two scenarios, call this method:
    /// ```
    /// define a as int = 5
    /// store b as int = 6
    /// ```
    /// @param name The name of the variable.
    /// @param type The type of the variable.
    /// @param literal The literal value of the variable.
    /// @throw CompilerError If the variable was already declared.
    static void init_constant_stack_variable(const std::string& name, const Type& type, std::unique_ptr<CustomNode>&& literal);

    /// @brief Clears the variables and the output file.
    static void clear(const std::string& output_file_path) noexcept;

    /// @brief Prints to the _start label an LDR operation.
    /// An LDR operation moves the address of a stack variable into a register.
    /// Therefore, to actually store the value of a stack variable into a register,
    /// we have to do two LDR operations:
    /// ```
    /// ldr rD, =variable_name
    /// ldr rD, [rD]
    /// ```
    /// @param rD The destination register.
    /// @param stack_variable_name The variable to get from the stack.
    /// @param pos_start The starting position of the variable name in the code itself.
    /// @param pos_end The ending position of the variable name in the code itself.
    /// @throw CompilerError If the given variable doesn't exist.
    static void ldr(unsigned int rD, const std::string& stack_variable_name, const Position& pos_start, const Position& pos_end);

    /// @brief Stores a value of register into the stack.
    /// The "sp" register doesn't need to be manually changed,
    /// there is an instruction to push something onto the stack:
    /// ```
    /// push {rS}
    /// ```
    /// @param rS The register in which the value to be stored is.
    /// @param var_name The name of the variable that's storing this value. Important for future access.
    /// @param type The type of the variable.
    /// @param value_node The value of this variable.
    static void push(unsigned int rS, const std::string& var_name, const Type& type, std::unique_ptr<CustomNode>&& value_node);

    /// @brief Prints a binary operation operation in the _start label:
    /// ```
    /// adds rD, rS, #literal
    /// ```
    /// It will automatically determine the exact operation (adds or subs or mul, etc.).
    /// @param assignment The type of variable assignment.
    /// @param rD The destination register.
    /// @param rS The source register.
    /// @param literal The literal to add.
    static void build_bin_op(const CompilerOperations::VariableAssignment& assignment, unsigned int rD, unsigned int rS, const CustomNode* literal);

    /// @brief Gets the Assembly equivalent of a type:
    /// - INTEGER = .word
    /// - STRING = .ascii
    /// @throw UndefinedBehaviorException If the type isn't supported.
    static std::string get_assembly_var_type(const Type& type);

    /// @brief Makes sure that a given variable exists.
    /// @param var_name The name of the variable.
    /// @param pos_start The starting position of the variable in the code itself.
    /// @param pos_end The ending position of the variable in the code itself.
    /// @throw CompilerError if the variable doesn't exist.
    static void require_variable(const std::string& var_name, const Position& pos_start, const Position& pos_end);

    /// @brief Makes sure that a given variable DOES NOT exist.
    /// @param var_name The name of the variable.
    /// @param pos_start The starting position of the variable in the code itself.
    /// @param pos_end The ending position of the variable in the code itself.
    /// @throw CompilerError if the variable exists.
    static void require_not_existing_variable(const std::string& var_name, const Position& pos_start, const Position& pos_end);

    /// @brief Finds the variable where a given variable is stored.
    /// With this I need to know if the value is stored in a register or in the stack,
    /// because it will change the syntax of the operation,
    /// hence the return type being "storage_t".
    /// @param name The name of the variable to find.
    /// @param pos_start The starting position of the variable in the code itself.
    /// @param pos_end The ending position of the variable in the code itself.
    /// @return Information about how this variable is stored in memory.
    /// @throw CompilerError if the variable doesn't exist.
    static storage_t& find(const std::string& name, const Position& pos_start, const Position& pos_end);

    /// @brief Is the register of index `reg` currently being occupied?
    /// The array of registers, when not given explicit default values,
    /// will try and initialize the struct "variable_t"
    /// with the individual default-constructued values,
    /// meaning that an empty register looks like this:
    /// `{ "", nullptr, indeterminate value, false }`
    static bool is_register_locked(unsigned int reg) noexcept;

    /// @brief Locks a register so that the get_free_register() method can get called
    /// several times in a row for operations that require multiple registers.
    /// @param reg The register to lock.
    static void lock_register(unsigned int reg) noexcept;

    /// @brief Releases a locked register.
    /// @param reg The register to unlock.
    /// @throw Exception if the register was not locked.
    static void release_register(unsigned int reg);

    /// @brief Sets the given register as temporary,
    /// meaning that this register is holding the result of an operation.
    static void assign_temporary_register(unsigned int reg) noexcept;

    /// @brief Sets the given register to the given variable name,
    /// meaning that the register is holding the value of a stack variable.
    static void assign_register(unsigned int reg, const std::string& var_name) noexcept;

    /// @brief Gets the index of a register without value.
    /// If all the registers are locked, then it will try and release one
    /// and store the value into the stack memory.
    /// @throw UndefinedBehaviorException If no registers are available (temporary)
    static unsigned int get_free_register();

    /// @brief When visiting a variabe assignment node, it's important to know
    /// if the variable is holding a simple literal, like in this example:
    /// ```
    /// store a as int = 5 // 5 is a literal
    /// ```
    /// Variables just holding a literal are directly written in the data section.
    /// @param value_node The value of the variable.
    /// @return `true` if it's a simple literal, `false` otherwise.
    static bool is_literal(const CustomNode* value_node) noexcept;

    /// @brief Is the node a binary operation?
    static bool is_binary_op(const CustomNode*) noexcept;

    /// @brief Gets the exact type of binary operation for a variable assignment.
    /// Meaning, if the binary operation is an addition,
    /// it will return the right type of VariableAssignment.
    /// @param op The binary operation node from a variable assignment.
    /// @param literals Is this binary operation only between two literals?
    /// @return The type of variable assignment when the value is a binary operation.
    /// @throw UndefinedBehaviorException If the operation isn't supported.
    static CompilerOperations::VariableAssignment get_type_of_bin_op(const BinaryOperationNode* op, bool literals);

    /// @brief Visits the nodes of the abstract syntax tree recursively.
    static void visit_ast(AST&&);

    /// @brief Converts the assignment of a variable into Assembly code.
    /// There are multiple types of assignment (depending on the value node).
    static void visit_VarAssignmentNode(std::unique_ptr<VarAssignmentNode>&&);

    /// @brief Converts the assignment of a constant into Assembly code.
    static void visit_DefineConstantNode(std::unique_ptr<DefineConstantNode>&&);

    /// @brief Converts an assignment into Assembly code.
    /// It can be a constant (via the define keyword) or a simple variable.
    /// It's the same logic for a variable and a constant.
    /// @param var_name The name of the variable/constant.
    /// @param var_type Its type
    /// @param value_node Its value
    static void visit_assignment(const std::string& var_name, const Type& var_type, std::unique_ptr<CustomNode>&& value_node);
};