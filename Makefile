.PHONY: cli file execute clean objects perf single_test tests

EXECUTABLE_DIRECTORY = build
EXECUTABLE_BASENAME = bangerking
EXECUTABLE_FULL_PATH = ${EXECUTABLE_DIRECTORY}/${EXECUTABLE_BASENAME}
OBJECTS_OUTPUT_DIR = build_objects
CPP_VERSION = c++20
GPP = g++ -std=${CPP_VERSION} -Wall -Wextra

source_files = $(shell find src -type f -name '*.cpp')

# This function gets all the source files
# of the project, but removes "cli.cpp" and "main.cpp"
# because they both contain a main function,
# and we don't want that when testing or measuring the performance.
define filter_main_functions
	main_file="src/main.cpp"; \
	cli_file="src/cli.cpp"; \
	files="$(call source_files)"; \
	files="$${files//$$main_file/}"; \
	files="$${files//$$cli_file/}"
endef

# Gets the source files and removes one file from the list.
# Useful because the CLI has its own main function,
# and "main.cpp" too
define filter_one_main_function
	main_file="$(1)"; \
	files="$(call source_files)"; \
	files="$${files//$$main_file/}"
endef

# Creates a local variable "all_object_files"
# that will find the list of object files contained in OBJECTS_OUTPUT_DIR
define get_object_files
	all_objects_files=$$(find "${OBJECTS_OUTPUT_DIR}" -type f -name "*.o")
endef

# This will compile the project and create
# an executable that would run the CLI for BangerKing.
# Note that this target also executes the executable.
cli:
	@mkdir -p ${EXECUTABLE_DIRECTORY}
	@$(call filter_one_main_function, src/main.cpp); \
	${GPP} $$files -o ${EXECUTABLE_FULL_PATH}
	@$(MAKE) execute

# Runs the CLI but uses the "-fsanitize=address" option of gpp.
# It's useful to locate memory issues more easily.
debug:
	@mkdir -p ${EXECUTABLE_DIRECTORY}
	@$(call filter_one_main_function, src/main.cpp); \
	${GPP} $$files -o ${EXECUTABLE_FULL_PATH} -fsanitize=address
	@$(MAKE) execute

# Usage:
# ```
# make file entrypoint=examples/main.bk
# ```
#	This will compile the project and create
# an executable that will interpret a BangerKing file.
entrypoint :=
file:
	@$(call filter_one_main_function, src/cli.cpp); \
	${GPP} $$files -o ${EXECUTABLE_FULL_PATH}
	@$(MAKE) execute

# This target executes the compiled executable, if there is one.
# If the project was compiled using `make file`
# then make sure to call this target like this:
# ```
# make execute entrypoint=examples/main.bk
# ```
# The entrypoint being the main program, the first file to execute.
execute: ${EXECUTABLE_FULL_PATH}
	@${EXECUTABLE_FULL_PATH} $(entrypoint)

# Use this target to compile every single c++ file into an object file.
# The object files don't get linked by g++.
# Basically, it makes sure to reconstruct the same structure that the c++ files have
# but it replaces each c++ file by its corresponding object file.
# It is used to measure the performance and for the tests,
# because in those cases each file have their own main function,
# therefore the project must be compiled first, separately.
#
# It also uses TPUT (or ANSI if not supported) to show what it's doing.
objects:
	@rm -fr ${OBJECTS_OUTPUT_DIR}
	@mkdir -p ${OBJECTS_OUTPUT_DIR}
	@tput_support=0; \
	if command -v tput >/dev/null 2>&1; then tput_support=1; fi; \
	$(filter_main_functions); \
	echo "Compiling project..."; \
	if [[ $$tput_support -eq 1 ]] ; then tput sc else echo -ne "\033[s" ; fi ; \
	for file in $$files ; do \
		base=$$(basename $$file); \
		dir=$$(dirname $$file); \
		if [[ "$$dir" == "src" ]] ; then \
			structure=""; \
		else \
			structure="$${dir#*/}"; \
		fi ; \
		mkdir -p "${OBJECTS_OUTPUT_DIR}/$$structure"; \
		echo "$$file..."; \
		g++ -c "$$file" -o "${OBJECTS_OUTPUT_DIR}/$$structure/$${base%.cpp}.o" -std=${CPP_VERSION}; \
		if [[ $$tput_support -eq 1 ]] ; then \
			tput rc; \
			tput el; \
		else \
			echo -ne "\033[u"; \
			echo -ne "\033[K"; \
		fi ; \
	done

# Use this target to measure the performance of the project.
# Basically, for each performance file, it compiles it and links it will the object files of the project,
# and then execute the performance file.
# Each file has its own main function, therefore it's necessary to compile the files one by one.
# The executable and the object files get removed afterwards.
perf:
	@$(MAKE) objects
	perfs="./tests/perf/*.perf.cpp"; \
	for perf in $$perfs; do \
		echo "Measuring performance of $$perf..."; \
		base=$$(basename "$$perf"); \
		g++ -c "$$perf" -o "${OBJECTS_OUTPUT_DIR}/$${base%.cpp}.o" -std=${CPP_VERSION}; \
		$(call get_object_files); \
		g++ $$all_objects_files -o ./compiled_perf -std=${CPP_VERSION}; \
		./compiled_perf; \
		rm -f "${OBJECTS_OUTPUT_DIR}/$${base%.cpp}.o"; \
	done; \
	rm -f ./compiled_perf; \
	rm -fr "${OBJECTS_OUTPUT_DIR}"

# Executes a single test.
# Usage:
# ```
# make single_test test=1-tokens
# ```
test :=
single_test:
	@rm -fr build_tests
	@mkdir -p build_tests
	@target=./tests/$(test).test.cpp; \
	echo "Testing $$target"; \
	$(filter_main_functions); \
	g++ $$files "$$target" -o build_tests/compiled_test -std=${CPP_VERSION} -fsanitize=address; \
	./build_tests/compiled_test; \
	rm -fr build_tests

# First, it compiles the entire project as object files without any "main" function.
# Then, it links the object files with the main function of each test,
# during this step, it compiles the test into an object file (the output is the name of the test, but with ".o" instead of ".cpp"),
# finally the object files get compiled altogether.
# The generated executable is executed, and then destroyed.
# The object files get deleted too.
# Note: the tests shall not be in any subdirectories.
tests:
	@$(MAKE) objects
	@tests="./tests/*.test.cpp"; \
	for test in $$tests; do \
		base=$$(basename "$$test"); \
		g++ -c "$$test" -o "${OBJECTS_OUTPUT_DIR}/$${base%.cpp}.o" -std=${CPP_VERSION}; \
		$(call get_object_files); \
		g++ $$all_objects_files -o ./compiled_test -std=${CPP_VERSION}; \
		./compiled_test; \
		rm -f "${OBJECTS_OUTPUT_DIR}/$${base%.cpp}.o"; \
	done; \
	rm -f ./compiled_test; \
	rm -fr "${OBJECTS_OUTPUT_DIR}"

# This target deletes the entire content of the build folder.
clean:
	@rm -fr ${EXECUTABLE_DIRECTORY}
	@mkdir -p ${EXECUTABLE_DIRECTORY}