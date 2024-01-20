#include <iostream>
#include "helper.hpp"
#include "../include/context.hpp"
using namespace std;

void test_context() {
  shared_ptr<Context> ctx = make_shared<Context>("<test>");
  assert(ctx->get_display_name() == "<test>");
  assert(!ctx->is_context_in("unknown"));
  assert(ctx->get_parent() == nullptr);
  assert(ctx->get_parent_entry_pos() == nullptr);
  assert(ctx->get_symbol_table() != nullptr);
  
  print_success_msg("simple context", 1);
}

void test_nested_context() {
  shared_ptr<Context> top_ctx = make_shared<Context>("<top>");
  shared_ptr<Context> bot_ctx = make_shared<Context>("<bottom>", top_ctx);
  assert(top_ctx->get_parent() == nullptr);
  assert(bot_ctx->get_parent().get() == top_ctx.get());
  assert(bot_ctx->is_context_in("<top>"));

  print_success_msg("nested context", 1);
}

void test_positions() {
  shared_ptr<Position> entry_pos = make_shared<Position>(0, 0, 0, "<hidden>");
  entry_pos->advance('1');
  shared_ptr<Context> ctx = make_shared<Context>("<test>", nullptr, entry_pos);
  assert(ctx->get_parent_entry_pos() != nullptr);
  assert(ctx->get_parent_entry_pos().get() == entry_pos.get());
  assert(ctx->get_parent_entry_pos()->get_idx() == 1);
  
  print_success_msg("entry position of context", 1);
}

int main() {
  print_title("Context tests...");

  test_context();
  test_nested_context();
  test_positions();

  print_success_msg("All \"Context\" tests successfully passed");
  return 0;
}