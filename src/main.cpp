// ONLY APPLIES WHEN USING MY OWN `Makefile`:
// Because the tests include their own main function (/tests/test_main.cpp),
// and that in order to run them I compile the whole project along with it,
// including this main function would create issues (we'd have two main functions).
// Therefore, when testing, the compilation command defines a preprocessor macro (TESTING_BK).
// Basically, this file will be ignored when testing.
#ifndef TESTING_BK

#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/context.hpp"
#include "../include/cli.hpp"
#include "../include/run.hpp"
#include "../include/runtime.hpp"
using namespace std;

// "argc" is the number of arguments passed to the executable.
// "argv" is the arguments themselves, of length "argc", with the first argument being the executable itself.
int main(int argc, char *argv[]) {
  if (argc > 2) {
    cerr << "Too many arguments passed to the main function." << endl;
    return 1;
  }

  // If there is no input file,
  // then just start the CLI
  // instead of returning an error.
  if (argc == 1) {
    cli();
    return 0;
  }

  string filename = argv[1];
  ifstream file(filename);

  if (!file.is_open()) {
    cerr << "The file " << filename << " doesn't exist." << endl;
    cout << "Current directory: " << std::filesystem::current_path() << endl;
    return 1;
  }

  file.seekg(0, std::ios::end);
  const streampos file_size = file.tellg(); // in bytes

  if (file_size == -1) {
    cerr << "Failed to determine the size of the input file." << endl;
    file.close();
    return 1;
  }

  if (static_cast<double>(file_size) > pow(2, 20)) {
    cerr << "The input file is too big (1MB maximum)" << endl;
    file.close();
    return 1;
  }

  file.seekg(0, std::ios::beg); // replace the cursor at the beginning to begin the process

  // The program reads the entire content of the file and stores it in memory.
  // This is fine for small files, but extremely large files will create problems.
  string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  cout << "Executing this code:" << endl;
  cout << content << endl;

  const shared_ptr<Context> global_ctx = make_shared<Context>(filename);

  // will do something with this
  const unique_ptr<const RuntimeResult> runtime_res = run(content, filename, global_ctx);

  file.close();

  return 0;
}

#endif