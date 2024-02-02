#pragma once

#include <fstream>

// Unfortunately casting to `fstream` from `ifstream`
// is a little bit complicated

unsigned long get_file_size(std::fstream& file);
unsigned long get_file_size(std::ifstream& file);