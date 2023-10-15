OUTPUT_DIR="build_perfs"

# Very much like "tests.sh"

./compile_objects.sh "$OUTPUT_DIR"

PERFS="./tests/perf/*.perf.cpp"

for perf in $PERFS ; do
  echo "Measuring performance of $perf..."
  base=$(basename "$perf")
  g++ -c "$perf" -o "$OUTPUT_DIR/${base%.cpp}.o" -std=c++20
  all_objects_files=$(find "$OUTPUT_DIR" -name "*.o")
  g++ $all_objects_files -o ./compiled_perf -std=c++20
  ./compiled_perf
  rm -f "$OUTPUT_DIR/${base%.cpp}.o"
done

rm -f ./compiled_perf
rm -fr "$OUTPUT_DIR"