#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/context.hpp"
#include "../include/run.hpp"
using namespace std;

// "argc" is the number of arguments passed to the executable.
// "argv" is the arguments themselves, of length "argc", with the first argument being the executable itself.
int main(int argc, char *argv[]) {
  if (argc > 2) {
    cerr << "Too many arguments passed to the main function." << endl;
    return 1;
  }

  string source_code = "";
  string filename = argv[1];
  ifstream file(filename);

  if (!file.is_open()) {
    cerr << "The file doesn't exist." << endl;
    return 1;
  }

  file.seekg(0, std::ios::end);
  streampos file_size = file.tellg(); // in bytes

  if (file_size == -1) {
    cerr << "Failed to determine the size of the input file." << endl;
    file.close();
    return 1;
  } else {
    cout << "file_size : " << file_size << endl;
    if (file_size > pow(2, 20)) {
      cerr << "The input file is too big (1MB maximum)" << endl;
      file.close();
      return 1;
    }
  }

  file.seekg(0, std::ios::beg); // replace the cursor at the beginning to begin the process

  // The program reads the entire content of the file and stores it in memory.
  // This is fine for small files, but extremely large files will create problems.
  string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  Context* global_ctx = new Context(filename);
  run(content, filename, global_ctx);

  file.close();

  return 0;
}
