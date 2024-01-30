.PHONY: file execute tests single_test perf clean

EXECUTABLE_DIRECTORY = build
EXECUTABLE_BASENAME = bangerking
EXECUTABLE_FULL_PATH = ${EXECUTABLE_DIRECTORY}/${EXECUTABLE_BASENAME}
CPP_VERSION = c++20
GPP = g++ -std=${CPP_VERSION} -Wall -Wextra

source_files = $(shell find src -type f -name '*.cpp')

# Usage:
# ```
# make file entrypoint=examples/main.bk
# ```
#	This will compile the project and create
# an executable that will interpret a BangerKing file.
entrypoint :=
file:
	@${GPP} $(call source_files) -o ${EXECUTABLE_FULL_PATH}
	@$(MAKE) execute

# This target executes the compiled executable, if there is one.
# And if you want to execute a file,
# make sure to call this target like this:
# ```
# make execute entrypoint=examples/main.bk
# ```
# The entrypoint being the main program, the first file to execute.
execute: ${EXECUTABLE_FULL_PATH}
	@${EXECUTABLE_FULL_PATH} $(entrypoint)

# Compiles all the test files with the main function
# that doctest is providing in tests/test_main.cpp.
# The command needs the preprocessor macro TESTING_BK
# so as to not compile the main function already
# present in the project (src folder).
tests:
	@g++ -DTESTING_BK -std=c++20 -Wall -Wextra -o my_tests tests/tests_main.cpp tests/*.test.cpp $(call source_files) && ./my_tests
	@rm -f ./my_tests

# Executes a single test.
# Usage:
# ```
# make single_test test=1-tokens
# ```
# The preprocessor macro TESTING_BK is necessary.
test :=
single_test:
	@echo "Testing ./tests/$(test).test.cpp"
	@g++ -DTESTING_BK -std=c++20 -Wall -Wextra -o my_tests tests/tests_main.cpp tests/$(test).test.cpp $(call source_files) && ./my_tests
	@rm -f ./my_tests

# The preprocessor macro TESTING_BK is necessary too,
# because it should not include the main function of the project (./src/main.cpp).
perf:
	@g++ -DTESTING_BK -std=c++20 -Wall -Wextra -o bk_perf $(call source_files) tests/perf/perf.cpp && ./bk_perf
	@rm -f ./bk_perf

# This target deletes the entire content of the build folder.
clean:
	@rm -fr ${EXECUTABLE_DIRECTORY}
	@mkdir -p ${EXECUTABLE_DIRECTORY}