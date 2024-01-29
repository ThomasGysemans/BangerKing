.PHONY: file execute clean objects perf single_test tests

EXECUTABLE_DIRECTORY = build
EXECUTABLE_BASENAME = bangerking
EXECUTABLE_FULL_PATH = ${EXECUTABLE_DIRECTORY}/${EXECUTABLE_BASENAME}
OBJECTS_OUTPUT_DIR = build_objects
CPP_VERSION = c++20
GPP = g++ -std=${CPP_VERSION} -Wall -Wextra

source_files = $(shell find src -type f -name '*.cpp')

# This function gets all the source files
# of the project, but removes "main.cpp"
# because it contains a main function,
# and we don't want that when testing
# or measuring the performance.
define filter_main_function
	main_file="src/main.cpp"; \
	files="$(call source_files)"; \
	files="$${files//$$main_file/}"
endef

# Creates a local variable "all_object_files"
# that will find the list of object files contained in OBJECTS_OUTPUT_DIR
define get_object_files
	all_objects_files=$$(find "${OBJECTS_OUTPUT_DIR}" -type f -name "*.o")
endef

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
	$(filter_main_function); \
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
	@perfs="./tests/perf/*.perf.cpp"; \
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
	$(filter_main_function); \
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