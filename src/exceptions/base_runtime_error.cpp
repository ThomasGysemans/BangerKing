#include "../../include/files.hpp"
#include "../../include/exceptions/base_runtime_error.hpp"
#include "../../include/utils/string_with_arrows.hpp"
using namespace std;

BaseRuntimeError::BaseRuntimeError(
  const Position& start,
  const Position& end,
  const string& name,
  const string& d,
  const shared_ptr<Context>& ctx
): CustomError(start, end, name, d), context(ctx) {}

string BaseRuntimeError::to_string() const {
  string result = generate_traceback();
  result += error_name + ": " + details;
  if (!READ_FILES.contains(pos_start.get_filename())) return result;
  result += "\n\n" + string_with_arrows(*(READ_FILES[pos_start.get_filename()]), pos_start, pos_end);
  return result;
}

string BaseRuntimeError::generate_traceback() const {
  shared_ptr<const Position> pos = make_shared<const Position>(pos_start);
  shared_ptr<const Context> ctx = context;
  string r;

  while (ctx != nullptr) {
    r = "    File " + pos->get_filename() + ", line " + std::to_string(pos->get_ln() + 1) + ", in " + ctx->get_display_name() + "\n" + r;
    pos = ctx->get_parent_entry_pos();
    ctx = ctx->get_parent();
  }

  return "Traceback (most recent call last):\n" + r;
}
