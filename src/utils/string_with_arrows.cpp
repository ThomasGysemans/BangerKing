#include "../../include/utils/string_with_arrows.hpp"

string string_with_arrows(const string& text, const Position& pos_start, const Position& pos_end) {
  string result = "";

  // Calculate the indices
  size_t last_line = result.substr(0, pos_start.get_idx()).find_last_of('\n');
  int index_start = last_line == string::npos ? 0 : last_line;
  size_t first_newline = text.find_first_of('\n', index_start + 1);
  int index_end = first_newline == string::npos ? text.length() : first_newline;

  // Generate each line
  int line_count = pos_end.get_ln() - pos_start.get_ln() + 1;
  for (int i = 0; i < line_count; ++i) {
    // Calculate line columns
    string line = text.substr(index_start, index_end);
    int col_start = i == 0 ? pos_start.get_col() : 0;
    int col_end = i == (line_count - 1) ? pos_end.get_col() : line.length() - 1;
    int n = col_end - col_start;

    // Append to result
    result += line + '\n';
    result += string(col_start, ' ') + string(n > 0 ? n : 1, '^');

    // Re-calculate indices
    index_start = index_end;
    index_end = text.find_first_of('\n', index_start + 1);
    if (index_end < 0) index_end = text.length();
  }

  remove_character(result, '\t');
  return result;
}
