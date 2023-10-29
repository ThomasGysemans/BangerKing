#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/context.hpp"
#include "../include/runtime.hpp"
#include "../include/run.hpp"
using namespace std;

// "argc" is the number of arguments passed to the executable.
// "argv" is the arguments themselves, of length "argc", with the first argument being the executable itself.
int main(int argc, char *argv[]) {
  if (argc > 2) {
    cerr << "Too many arguments passed to the main function." << endl;
    return 1;
  }

  if (argc == 1) {
    cerr << "Missing input file." << endl;
    return 1;
  }

  string source_code = "";
  string filename = argv[1];
  ifstream file(filename);

  if (!file.is_open()) {
    cerr << "The file " << filename << " doesn't exist." << endl;
    cout << "Current directory: " << std::filesystem::current_path() << endl;
    return 1;
  }

  file.seekg(0, std::ios::end);
  streampos file_size = file.tellg(); // in bytes

  if (file_size == -1) {
    cerr << "Failed to determine the size of the input file." << endl;
    file.close();
    return 1;
  } else {
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

  cout << "Executing this code:" << endl;
  cout << content << endl;

  Context* global_ctx = new Context(filename);
  const RuntimeResult* res = run(content, filename, global_ctx);

  file.close();

  delete global_ctx;
  delete res;

  return 0;
}
