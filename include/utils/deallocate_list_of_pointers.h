#ifndef DEALLOCATE_LIST_OF_POINTS_H
#define DEALLOCATE_LIST_OF_POINTS_H

#include <iostream>
#include <list>
using namespace std;

/// @brief Deallocates the content of a list that contains a list of pointers to objects
/// @tparam T The type of pointed values
/// @param l The list
template <typename T>
void deallocate_list_of_pointers(list<T*>& l) {
  for (auto iter = l.begin(); iter != l.end(); ++iter) delete *iter;
  l.clear();
}

#endif