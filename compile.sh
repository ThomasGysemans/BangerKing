rm -fr build
mkdir -p build
SOURCE_FILES=$(find src -name '*.cpp')
if [[ "$1" == "debug" ]] ; then
  MAIN_FILE_TO_REMOVE="src/main.cpp"
  SOURCE_FILES=${SOURCE_FILES//$MAIN_FILE_TO_REMOVE/} # we want to run the cli
  g++ $SOURCE_FILES -o build/bangerking -std=c++20 -Wall -Wextra -fsanitize=address
else
  MAIN_FILE_TO_REMOVE=""
  if [[ ! -z "$1" ]] ; then # here we want to remove the CLI because the compilation command asks to run a banger king file
    MAIN_FILE_TO_REMOVE="src/cli.cpp"
  else # if there is no input file, then run the cli, so remove "main.cpp"
    MAIN_FILE_TO_REMOVE="src/main.cpp"
  fi
  SOURCE_FILES=${SOURCE_FILES//$MAIN_FILE_TO_REMOVE/} # we remove the undesired file because it contains a `main` function
  g++ $SOURCE_FILES -o build/bangerking -std=c++20 -Wall -Wextra
fi