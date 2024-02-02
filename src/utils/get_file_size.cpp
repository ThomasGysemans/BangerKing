#include "../../include/utils/get_file_size.hpp"
using namespace std;

unsigned long get_file_size(fstream& file) {
    file.seekg(0, ios::end);
    const auto file_size = file.tellg(); // in bytes
    if (file_size == -1) {
        return 0;
    }
    file.seekg(0, ios::beg);
    return file_size;
}

unsigned long get_file_size(ifstream& file) {
    file.seekg(0, ios::end);
    const auto file_size = file.tellg(); // in bytes
    if (file_size == -1) {
        return 0;
    }
    file.seekg(0, ios::beg);
    return file_size;
}