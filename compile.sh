rm -fr build
mkdir -p build
SOURCE_FILES=$(find ./src -name '*.cpp')
g++ $SOURCE_FILES -o build/bangerking -std=c++20 -Wall -Wextra