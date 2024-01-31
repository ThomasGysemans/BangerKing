# Reminders:
# - Initialize CMake
# cmake -S . -B build/
# - Run CMake
# cmake --build build/
# Run the executable:
# ./build/bangerking

cmake_minimum_required(VERSION 3.20)

project(BangerKing)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_OSX_ARCHITECTURES "arm64")  # For Apple Silicon architecture

add_library(
  bangerking_src STATIC
  src/miscellaneous.cpp
  src/types.cpp
  src/interpreter.cpp
  src/nodes/add_node.cpp
  src/nodes/define_constant_node.cpp
  src/nodes/power_node.cpp
  src/nodes/var_assignment_node.cpp
  src/nodes/binary_operation.cpp
  src/nodes/divide_node.cpp
  src/nodes/var_access_node.cpp
  src/nodes/minus_node.cpp
  src/nodes/multiply_node.cpp
  src/nodes/string_node.cpp
  src/nodes/double_node.cpp
  src/nodes/modulo_node.cpp
  src/nodes/and_node.cpp
  src/nodes/substract_node.cpp
  src/nodes/not_node.cpp
  src/nodes/list_node.cpp
  src/nodes/var_modify_node.cpp
  src/nodes/custom_node.cpp
  src/nodes/plus_node.cpp
  src/nodes/boolean_node.cpp
  src/nodes/integer_node.cpp
  src/nodes/or_node.cpp
  src/context.cpp
  src/run.cpp
  src/values/string.cpp
  src/values/value.cpp
  src/values/list.cpp
  src/values/integer.cpp
  src/values/boolean.cpp
  src/values/double.cpp
  src/lexer.cpp
  src/utils/double_to_string.cpp
  src/utils/read_entire_file.cpp
  src/utils/string_with_arrows.cpp
  src/exceptions/type_overflow.cpp
  src/exceptions/arithmetic_error.cpp
  src/exceptions/exception.cpp
  src/exceptions/illegal_string.cpp
  src/exceptions/undefined_behavior.cpp
  src/exceptions/invalid_syntax_error.cpp
  src/exceptions/runtime_error.cpp
  src/exceptions/unclosed_string_error.cpp
  src/exceptions/expected_char_error.cpp
  src/exceptions/base_runtime_error.cpp
  src/exceptions/custom_error.cpp
  src/exceptions/illegal_char_error.cpp
  src/exceptions/type_error.cpp
  src/files.cpp
  src/symbol_table.cpp
  src/cli.cpp
  src/runtime.cpp
  src/parser.cpp
  src/token.cpp
  src/position.cpp
  src/debug/print_tokens.cpp
  src/debug/compare_tokens.cpp
)

# I really do not like this solution,
# but it's the only one I have to make sure
# that the use of CMake is actually worth it.
# It's supposed to make compiling more efficient,
# and it's supposed to make port to other OS more easily.
# Obviously this gigantic list of files was not written by hand,
# just execute this command in a terminal:
# find src -type f -name '*.cpp'
# and copy the output after "bangerking", below.
add_executable(
  bangerking
  src/main.cpp # the main function specific to this executable
)

add_executable(
  bangerking_tests
  tests/tests_main.cpp
  tests/0-context.test.cpp
  tests/0-miscellaneous.test.cpp
  tests/0-positions.test.cpp
  tests/0-runtime.test.cpp
  tests/0-symbol-table.test.cpp
  tests/1-tokens.test.cpp
  tests/2-lexer.test.cpp
  tests/3-parser.test.cpp
  tests/4-values.test.cpp
  tests/5-interpreter.test.cpp
  tests/6-run.test.cpp
)

add_executable(
  bangerking_perfs
  tests/perf/perf.cpp
)

target_link_libraries(bangerking PRIVATE bangerking_src stdc++)
target_link_libraries(bangerking_tests PRIVATE bangerking_src stdc++)
target_link_libraries(bangerking_perfs PRIVATE bangerking_src stdc++)