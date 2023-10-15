OUTPUT_DIR="build_tests"

# Step 1: Compile the entire project as object files without any "main" function
./compile_objects.sh "$OUTPUT_DIR"
echo "Compiling tests..."

TESTS="./tests/*.test.cpp" # the tests should not be in any subdirectories

# Step 2: Link the object files with the main function of each test
for test in $TESTS; do
  base=$(basename "$test")
  g++ -c "$test" -o "$OUTPUT_DIR/${base%.cpp}.o" -std=c++20 # "%.cpp" is to remove the ".cpp" extension in order to replace it with ".o"
  all_objects_files=$(find "$OUTPUT_DIR" -name "*.o") # all the object files previously compiled
  g++ $all_objects_files -o ./compiled_test -std=c++20 # link the test (the one with the "main" function) with the project
  ./compiled_test # run the test
  rm -f "$OUTPUT_DIR/${base%.cpp}.o" # remove the main function (the test), indeed we don't want multiple main functions for the next iterations
done

# Clean up
rm -f ./compiled_test
rm -fr "$OUTPUT_DIR"