#pragma once

#include <list>
#include <vector>
using namespace std;

// As a reminder, templated functions must be defined in the header file
// or a "undefined symbol" error will always show up at compile time.

// These functions are useful when I have a list of pointers
// and I want to deallocate each value of the list.
// When the list isn't constant, it gets cleared,
// and if it's a constant, then it doesn't get cleared.

template <typename T>
void deallocate_list_of_pointers(list<T*>& l) {
  for (auto iter = l.begin(); iter != l.end(); ++iter) delete *iter;
  l.clear();
}

template <typename T>
void deallocate_const_list_of_pointers(const list<T*>& l) {
  for (auto iter = l.begin(); iter != l.end(); ++iter) delete *iter;
}

template <typename T>
void deallocate_const_vector_of_pointers(const vector<T*>& l) {
  for (auto iter = l.begin(); iter != l.end(); ++iter) delete *iter;
}