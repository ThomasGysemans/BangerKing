mkdir -p build_tests

# if "$1" is empty,
# then execute all tests
if [[ -z "$1" ]] ; then
  F="./tests/*.test.cpp"
  for f in $F ; do
    echo "Testing $f... "
    g++ "$f" -o build_tests/compiled_test -std=c++20 && ./build_tests/compiled_test
  done
else
  target="./tests/$1.test.cpp"
  echo "Testing $target"
  g++ "$target" -o build_tests/compiled_test -std=c++20 && ./build_tests/compiled_test
fi

rm -fr build_tests