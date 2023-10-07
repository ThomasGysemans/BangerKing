mkdir -p build_perfs

F="./tests/perf/*.perf.cpp"
for f in $F ; do
  echo "Measuring performance of $f... "
  g++ "$f" -o build_perfs/compiled_test -std=c++20 && ./build_perfs/compiled_test
done

rm -fr build_perfs