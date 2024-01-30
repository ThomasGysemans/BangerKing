#include <iostream>
#include "doctest.h"
#include "../include/context.hpp"
using namespace std;

DOCTEST_TEST_SUITE("Context") {
  SCENARIO("simple context") {
    shared_ptr<Context> ctx = make_shared<Context>("<test>");
    CHECK(ctx->get_display_name() == "<test>");
    CHECK(!ctx->is_context_in("unknown"));
    CHECK(ctx->get_parent() == nullptr);
    CHECK(ctx->get_parent_entry_pos() == nullptr);
    CHECK(ctx->get_symbol_table() != nullptr);
  }

  SCENARIO("nested context") {
    shared_ptr<Context> top_ctx = make_shared<Context>("<top>");
    shared_ptr<Context> bot_ctx = make_shared<Context>("<bottom>", top_ctx);
    CHECK(top_ctx->get_parent() == nullptr);
    CHECK(bot_ctx->get_parent().get() == top_ctx.get());
    CHECK(bot_ctx->is_context_in("<top>"));
  }

  SCENARIO("positions") {
    shared_ptr<Position> entry_pos = make_shared<Position>(0, 0, 0, "<hidden>");
    entry_pos->advance('1');
    shared_ptr<Context> ctx = make_shared<Context>("<test>", nullptr, entry_pos);
    assert(ctx->get_parent_entry_pos() != nullptr);
    assert(ctx->get_parent_entry_pos().get() == entry_pos.get());
    assert(ctx->get_parent_entry_pos()->get_idx() == 1);
  }
}