#pragma once

#include <list>
using namespace std;

// As a reminder, templated functions must be defined in the header file
// or a "undefined symbol" error will always show up at compile time.

template <typename T>
void deallocate_list_of_pointers(list<T*>& l) {
  for (auto iter = l.begin(); iter != l.end(); ++iter) delete *iter;
  l.clear();
}
