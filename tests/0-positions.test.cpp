#include <iostream>
#include "doctest.h"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/position.hpp"
using namespace std;

DOCTEST_TEST_SUITE("Positions") {
  SCENARIO("position") {
    Position pos = Position::getDefaultPos();
    CHECK(pos.get_idx() == 0);
    CHECK(pos.get_col() == 0);
    CHECK(pos.get_ln() == 0);
    CHECK(pos.get_filename() == "<hidden>");
  }

  SCENARIO("advanced position") {
    Position pos = Position::getDefaultPos();
    pos.advance('5');
    CHECK(pos.get_idx() == 1);
    CHECK(pos.get_col() == 1);
    CHECK(pos.get_ln() == 0);
  }

  SCENARIO("multiline position") {
    Position pos = Position::getDefaultPos();
    pos.advance('5');
    pos.advance('\n');
    CHECK(pos.get_idx() == 2);
    CHECK(pos.get_col() == 0);
    CHECK(pos.get_ln() == 1);
  }

  SCENARIO("to string") {
    Position pos = Position::getDefaultPos();
    pos.advance('6');
    pos.advance('0');
    pos.advance('\n');

    string str = pos.to_string();
    CHECK(str == "1:0, idx=3");
  }

  SCENARIO("equals") {
    Position pos1 = Position::getDefaultPos();
    Position pos2 = Position::getDefaultPos();
    CHECK(pos1.equals(pos1));
    CHECK(pos1.equals(pos2));

    pos2.advance('5');
    CHECK(!pos1.equals(pos2));

    pos1.advance('5');
    CHECK(pos1.equals(pos2));

    pos1.advance('\n');
    pos2.advance('\n');
    CHECK(pos1.equals(pos2));
  }
}