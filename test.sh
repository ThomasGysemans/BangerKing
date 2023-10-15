# Since each test has its own "main" function,
# we remove from the project the actual "main" function that starts the CLI
SOURCE_FILES=$(find src -name '*.cpp')
MAIN_FILE="src/main.cpp"
SOURCE_FILES=${SOURCE_FILES//$MAIN_FILE/}

if [[ -z "$1" ]] ; then
  echo "Missing test file"
  exit 1
fi

rm -fr build_tests
mkdir -p build_tests

target="./tests/$1.test.cpp"
echo "Testing $target"
g++ $SOURCE_FILES "$target" -o build_tests/compiled_test -std=c++20 && ./build_tests/compiled_test

rm -fr build_tests