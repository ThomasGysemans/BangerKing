#include "../../include/debug/compare_tokens.hpp"
using namespace std;

template<typename T>
bool list_equals(const list<T>& l1, const list<T>& l2) {
  if (l1.size() != l2.size()) return false;
  // note that "typename" is necessary to make C++ understand
  // that the type `list<T>` refers to the template's type parameter
  typename list<T>::const_iterator iter1 = l1.begin(); // `const_iterator` is necessary because I'm using constant lists
  typename list<T>::const_iterator iter2 = l2.begin(); // indeed, `iterator` alone allows you the modify the list
  while (iter1 != l1.end()) {
    if (!(*iter1 == *iter2)) {
      return false;
    }
    ++iter1;
    ++iter2;
  }
  return true;
}

bool compare_tokens(const list<TokenType>& expected, const list<unique_ptr<const Token>>& actual) {
  list<TokenType> actual_tokens_list;
  list<unique_ptr<const Token>>::const_iterator iter = actual.begin();
  while (iter != actual.end()) {
    actual_tokens_list.push_back((*iter)->getType());
    ++iter;
  }
  return list_equals<TokenType>(expected, actual_tokens_list);
}