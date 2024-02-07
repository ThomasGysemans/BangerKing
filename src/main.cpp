// ONLY APPLIES WHEN USING MY OWN `Makefile`:
// Because the tests include their own main function (/tests/test_main.cpp),
// and that in order to run them I compile the whole project along with it,
// including this main function would create issues (we'd have two main functions).
// Therefore, when testing, the compilation command defines a preprocessor macro (TESTING_BK).
// Basically, this file will be ignored when testing.
#ifndef TESTING_BK
#include "../include/parser.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/utils/get_file_size.hpp"
#include "../include/context.hpp"
#include "../include/cli.hpp"
#include "../include/run.hpp"
#include "../include/compiler.hpp"
#include "../include/runtime.hpp"
using namespace std;

// "argc" is the number of arguments passed to the executable.
// "argv" is the arguments themselves, of length "argc", with the first argument being the executable itself.
int main(int argc, char *argv[]) {
  if (argc >= 3 && string(argv[1]) == "--compile") {
    try {
      const string bk_file = string(argv[2]);
      const string output_path = argc == 4 ? argv[3] : ("./" + bk_file.substr(0, bk_file.find_last_of('.')) + ".s");
      Parser parser = Parser::initFile(make_shared<string>(), bk_file);
      Compiler::compile(parser.parse(), output_path);
    } catch (Exception& e) {
      cerr << "An error occured during compilation of " << argv[2] << endl;
      cerr << e.to_string() << endl;
      return 1;
    }
    return 0;
  }

  if (argc > 2) {
    cerr << "Too many arguments passed to the main function." << endl;
    cerr << "Usage:" << endl;
    cerr << "Start the cli: " << argv[0] << endl;
    cerr << "Interpret a file: " << argv[0] << " file.bk" << endl;
    cerr << "Compile a file: " << argv[0] << " --compile file.bk [output_path]" << endl;
    return 1;
  }

  // If there is no input file,
  // then just start the CLI
  // instead of returning an error.
  if (argc == 1) {
    cli();
    return 0;
  }

  const string filename = argv[1];
  ifstream file(filename);

  if (!file.is_open()) {
    cerr << "The file " << filename << " doesn't exist." << endl;
    cout << "Current directory: " << std::filesystem::current_path() << endl;
    return 1;
  }

  const unsigned long file_size = get_file_size(file); // in bytes

  if (file_size == 0) {
    cerr << "Failed to determine the size of the input file." << endl;
    file.close();
    return 1;
  }

  if (static_cast<double>(file_size) > pow(2, 20)) {
    cerr << "The input file is too big (1MB maximum)" << endl;
    file.close();
    return 1;
  }

  // "main()" should not print anything on its own,
  // but right now it's useful to see what's happening when testing.
  cout << "Reading code..." << endl;

  const shared_ptr<Context> global_ctx = make_shared<Context>(filename);

  // will do something with this
  runFile(filename, global_ctx);

  file.close();

  cout << "Everything went well" << endl;

  return 0;
}

#endif