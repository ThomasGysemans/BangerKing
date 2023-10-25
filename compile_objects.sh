# This file compiles all the project files into object files, without "main.cpp".
# It's important in order to measure performance and execute tests.
# Give as first argument the value of "OUTPUT_DIR".

if [[ -z "$1" ]] ; then
  echo "Missing output directory as first argument of this script."
  exit 1
fi

OUTPUT_DIR="$1"

# Make sure there is no residual files
rm -fr "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

# In a nutshell, "tput" is the command to use over ANSI escape sequences to ensure a wider compability.
# Here, we make sure it's available. If not, we'll print ANSI sequences.
# If the current terminal doesn't support any of them, then the sequences should be hidden, and it shouldn't be a problem.
TPUT_SUPPORT=0
if command -v tput >/dev/null 2>&1; then
  TPUT_SUPPORT=1
fi

function save_cursor_position {
  if [[ $TPUT_SUPPORT -eq 1 ]] ; then
    tput sc
  else
    echo -ne "\033[s"
  fi
}

function erase_line {
  if [[ $TPUT_SUPPORT -eq 1 ]] ; then
    tput el
  else
    echo -ne "\033[K"
  fi
}

function restore_cursor_position {
  if [[ $TPUT_SUPPORT -eq 1 ]] ; then
    tput rc
  else
    echo -ne "\033[u"
  fi
}

# Since each test has its own "main" function,
# we remove from the project the actual "main" function that starts the CLI
SOURCE_FILES=$(find src -name '*.cpp')
MAIN_FILE="src/main.cpp" # the program that runs a file
CLI_FILE="src/cli.cpp" # the program that starts the CLI
SOURCE_FILES=${SOURCE_FILES//$MAIN_FILE/}
SOURCE_FILES=${SOURCE_FILES//$CLI_FILE/}

# The project must be compiled first,
# but without the "main" function.
# Indeed, each test has its own "main" function.

echo "Compiling project..."
save_cursor_position

# Step 1: Compile source files (excluding main) into object files
for file in $SOURCE_FILES; do
  base=$(basename $file)
  dir=$(dirname $file) # removes the basename from the path (the named of the file itself, so it removes "something.cpp")
  if [[ "$dir" == "src" ]] ; then
    structure=""
  else
    # removes the first directory of the given path ("src/")
    structure="${dir#*/}"
  fi
  mkdir -p "$OUTPUT_DIR/$structure" # re-create the same structure than in "src"
  echo "$file..."
  g++ -c "$file" -o "$OUTPUT_DIR/$structure/${base%.cpp}.o" -std=c++20 # "-c" is to ask g++ to only compile without linking all the results
  restore_cursor_position
  erase_line
done

echo "Done."