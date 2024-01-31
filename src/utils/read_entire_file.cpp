#include <fstream>
#include <iostream>
#include "../../include/utils/read_entire_file.hpp"
using namespace std;

string read_entire_file(const string& path) {
  ifstream file(path);

  if (!file.is_open()) {
    cerr << "The file " << path << " doesn't exist." << endl;
    cout << "Current directory: " << filesystem::current_path() << endl;
    exit(1);
  }

  string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  file.close();

  return content;
}