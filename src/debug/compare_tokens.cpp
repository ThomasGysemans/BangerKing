#include "../../include/debug/compare_tokens.hpp"
using namespace std;

template<typename T>
bool list_equals(const list<T>& l1, const list<T>& l2) {
  if (l1.size() != l2.size()) return false;
  // note that "typename" is necessary to make C++ understand
  // that the type `list<T>` refers to the template's type parameter
  auto iter1 = l1.begin(); // `const_iterator` is necessary because I'm using constant lists
  auto iter2 = l2.begin(); // indeed, `iterator` alone allows you the modify the list
  while (iter1 != l1.end()) {
    if (!(*iter1 == *iter2)) {
      return false;
    }
    ++iter1;
    ++iter2;
  }
  return true;
}

bool compare_tokens(const list<TokenType>& expected, const list<shared_ptr<const Token>>& actual) {
  list<TokenType> actual_tokens_list;
  auto iter = actual.begin();
  while (iter != actual.end()) {
    actual_tokens_list.push_back((*iter)->getType());
    ++iter;
  }
  return list_equals<TokenType>(expected, actual_tokens_list);
}