#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include "position.h"

using namespace std;

template <class T>
class Value {
  string type;
  T actual_value = NULL;
  Position pos_start;
  Position pos_end;

  public:
    /**
     * @param t The type of the value.
     */
    Value(string t): type(t) {
      pos_start = Position::getDefaultPos();
      pos_end = Position::getDefaultPos();
    }

    /**
     * Sets the position of the value in the code.
     * @param pos_start Starting position of the value.
     * @param pos_end End position of the value.
     */
    Value& set_pos(const Position& pos_start, const Position& pos_end) {
      this.pos_start = pos_start;
      this.pos_end = pos_end;
      return *this;
    }

    /**
     * Indicates how this particular value can evaluate to true if used in a condition.
     * @returns A boolean
     */
    virtual bool is_true() const {
      return true;
    }

    /**
     * Returns what needs to be use for comparison with another Value.
     */
    virtual T equivalent() const {
      // TODO: throw unimplemented exception?
      return NULL;
    }

    Value<T> copy() const {
      return Value(*this);
    }

    /**
     * Transforms the value into a string.
     * @returns The string representation of this value.
     */
    virtual string to_string() const {
      if (actual_value == NULL) {
        return "NULL";
      }
      return std::to_string(actual_value);
    }

    /**
     * We don't always want the same string representation when used in a concatenation or printed to the screen.
     * @returns The string to be printed when used on a println().
     */
    virtual string repr() const {
      return this.to_string();
    }
};

#endif