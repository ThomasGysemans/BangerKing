rm -fr build
mkdir -p build
SOURCE_FILES=$(find ./src -name '*.cpp')
if [[ "$1" == "debug" ]] ; then
  g++ $SOURCE_FILES -o build/bangerking -std=c++20 -Wall -Wextra -fsanitize=address
else
  g++ $SOURCE_FILES -o build/bangerking -std=c++20 -Wall -Wextra
fi