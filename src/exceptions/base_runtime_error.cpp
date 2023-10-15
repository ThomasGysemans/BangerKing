#include "../../include/exceptions/base_runtime_error.hpp"
#include "../../include/utils/string_with_arrows.hpp"
#include "../../include/files.hpp"

BaseRuntime::BaseRuntime(
  const Position& start,
  const Position& end,
  const string& name,
  const string& d,
  const Context* ctx
): CustomError(start, end, name, d), context(ctx) {}

string BaseRuntime::to_string() const {
  string result = generate_traceback();
  result += error_name + ": " + details + "\n\n";
  result += string_with_arrows(*(READ_FILES[pos_start.get_filename()]), pos_start, pos_end);
  return result;
}

string BaseRuntime::generate_traceback() const {
  const Position* pos = &pos_start;
  const Context* ctx = context;
  string r = "";

  while (ctx != nullptr) {
    r = "    File " + pos->get_filename() + ", line " + std::to_string(pos->get_ln() + 1) + ", in " + ctx->get_display_name() + "\n" + r;
    pos = ctx->get_parent_entry_pos();
    ctx = ctx->get_parent();
  }

  return "Traceback (most recent call last):\n" + r;
}
