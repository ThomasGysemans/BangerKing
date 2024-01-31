#include "../../include/utils/string_with_arrows.hpp"
using namespace std;

string string_with_arrows(const string& text, const Position& pos_start, const Position& pos_end) {
  string result;

  // Calculate the indices
  const size_t last_line = result.substr(0, pos_start.get_idx()).find_last_of('\n');
  unsigned int index_start = last_line == string::npos ? 0 : static_cast<unsigned int>(last_line);
  const size_t first_newline = text.find_first_of('\n', index_start + 1);
  auto index_end = static_cast<unsigned int>(first_newline == string::npos ? text.length() : first_newline);

  // Generate each line
  const unsigned int line_count = pos_end.get_ln() - pos_start.get_ln() + 1;
  for (unsigned int i = 0; i < line_count; ++i) {
    // Calculate line columns
    string line = text.substr(index_start, index_end);
    const unsigned int col_start = i == 0 ? pos_start.get_col() : 0;
    const unsigned int col_end = i == (line_count - 1) ? pos_end.get_col() : line.length() - 1;
    const unsigned int n = col_end - col_start;

    // Append to result
    result += line + '\n';
    result += string(col_start, ' ') + string(n > 0 ? n : 1, '^');

    // Re-calculate indices
    index_start = index_end;
    const size_t next_line = text.find_first_of('\n', index_start + 1);
    index_end = static_cast<int>(next_line == string::npos ? text.length() : next_line);
  }

  remove_character(result, '\t');
  return result;
}
