#include <iostream>
#include <fstream>
#include <mach/mach.h>
#include "../../include/token.hpp"
#include "../../include/lexer.hpp"
#include "../../include/parser.hpp"
#include "../../include/debug/print_tokens.hpp"
#include "../../include/utils/read_entire_file.hpp"
#include "../../include/nodes/compositer.hpp"
#include "../../include/interpreter.hpp"
#include "../../include/utils/double_to_string.hpp"
using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

using lexer_rt = list<unique_ptr<const Token>>;
using parser_rt = unique_ptr<ListNode>;

struct nice_time_t {
  string year;
  string month;
  string day;
  string hour;
  string minute;
  string seconds;
};

struct measurements_t {
  double time;
  double memory;
};

constexpr double treshold = 5.0; // above this amount of milliseconds, I consider that there is a performance issue.
const string ANSI_RED = "\e[0;31m";
const string ANSI_GREEN = "\e[0;32m";
const string ANSI_RESET = "\e[0m";

double get_milliseconds(const chrono::steady_clock::time_point& t1, const chrono::steady_clock::time_point& t2) {
  const duration<double, std::milli> ms_double = t2 - t1;
  return ms_double.count();
}

string get_success(const string& message) {
  return ANSI_GREEN + message + ANSI_RESET;
}

string get_failure(const string& message) {
  return ANSI_RED + message + ANSI_RESET;
}

void show_results(const string& name, const measurements_t& results) {
  // If it took more than `treshold` milliseconds,
  // then show red result
  // otherwise show it in green
  const string str_time = double_to_string(results.time);
  const string answer = results.time > treshold ? get_failure(str_time) : get_success(str_time);
  cout << name << " total performance test took " << answer << " ms and " << double_to_string(results.memory) << " bytes of memory" << endl;
}

string pad(const int number) {
  if (number < 10) {
    return "0" + std::to_string(number);
  } else {
    return std::to_string(number);
  }
}

nice_time_t get_localtime() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
  const std::tm* localTime = std::localtime(&currentTime);

  nice_time_t nice_time;

  nice_time.year = std::to_string(localTime->tm_year + 1900);  // Years since 1900, because C starts counting at 1900, why not
  nice_time.month = pad(localTime->tm_mon + 1);                // Month (0-based, so add 1)
  nice_time.day = pad(localTime->tm_mday);                     // Day of the month
  nice_time.hour = pad(localTime->tm_hour);
  nice_time.minute = pad(localTime->tm_min);
  nice_time.seconds = pad(localTime->tm_sec);

  // It outputs the format:
  // yyyy-mm-dd+hh-mm
  return nice_time;
}

size_t get_current_memory_usage() {
  task_vm_info_data_t vmInfo;
  mach_msg_type_number_t infoCount = TASK_VM_INFO_COUNT;
  if (task_info(mach_task_self(), TASK_VM_INFO, (task_info_t)&vmInfo, &infoCount) == KERN_SUCCESS) {
    return vmInfo.phys_footprint;
  }
  return 0; // Failed to read memory usage
}

measurements_t measure_lexer(const string& source_code, int* number_of_tokens) {
  const auto lexer_musage1 = get_current_memory_usage();
  const auto l1 = high_resolution_clock::now();
  const auto lexer = Lexer::readCLI(source_code);
  list<shared_ptr<const Token>> tokens;
  while (lexer->hasMoreTokens()) {
    tokens.push_back(lexer->get_next_token());
  }
  const auto l2 = high_resolution_clock::now();
  const auto lexer_musage2 = get_current_memory_usage();
  measurements_t results{};
  results.time = get_milliseconds(l1, l2);
  results.memory = static_cast<double>(lexer_musage2 - lexer_musage1);
  cout << "which produced " << tokens.size() << " tokens" << endl;
  *number_of_tokens = static_cast<int>(tokens.size());
  return results;
}

measurements_t measure_parser(const string& source_code) {
  const auto parser_musage1 = get_current_memory_usage();
  const auto p1 = high_resolution_clock::now();
  Parser parser = Parser::initCLI(source_code);
  auto ast = parser.parse();
  const auto p2 = high_resolution_clock::now();
  const auto parser_musage2 = get_current_memory_usage();
  ast.reset();
  measurements_t results{};
  results.time = get_milliseconds(p1, p2);
  results.memory = static_cast<double>(parser_musage2 - parser_musage1);
  return results;
}

measurements_t measure_interpreter(const string& source_code) {
  const auto interpreter_musage1 = get_current_memory_usage();
  const auto i1 = high_resolution_clock::now();
  const shared_ptr<Context> ctx = make_shared<Context>("<perf>");
  Parser parser = Parser::initCLI(source_code);
  Interpreter::set_shared_ctx(ctx);
  Interpreter::visit(parser.parse());
  const auto i2 = high_resolution_clock::now();
  const auto interpreter_musage2 = get_current_memory_usage();
  measurements_t results{};
  results.time = get_milliseconds(i1, i2);
  results.memory = static_cast<double>(interpreter_musage2 - interpreter_musage1);
  return results;
}

string markdown_table_line(const string& name, const measurements_t& results) {
  const double kbi = results.memory / 1024;
  return "|" + name + "(CLI, total)|" + double_to_string(results.time) + " ms|" + double_to_string(results.memory) + " bytes, " + double_to_string(kbi) + " kbi|";
}

int main() {
  cout << "Testing individual parts first..." << endl;
  const auto r1 = high_resolution_clock::now();
  const string source_code = read_entire_file("../tests/perf/source_code.txt");
  const auto r2 = high_resolution_clock::now();
  const double rt = get_milliseconds(r1, r2);

  cout << "Reading the source code took " << rt << " ms" << endl;
  cout << "The source code contains " << source_code.length() << " characters (" << source_code.length() * sizeof(char) << " bytes)" << endl;

  int number_of_tokens = 0;
  const measurements_t lexer_measurements = measure_lexer(source_code, &number_of_tokens);
  const measurements_t parser_measurements = measure_parser(source_code);
  const measurements_t interpreter_measurements = measure_interpreter(source_code);

  show_results("Lexer", lexer_measurements);
  show_results("Parser", parser_measurements); // sometimes the memory usage of the lexer and the parser are exactly the same, and I've no idea why
  show_results("Interpreter", interpreter_measurements);

  // Writing a log file with Markdown syntax.
  // I know the way I'm writing the file is kinda terrible,
  // but it really doesn't matter.

  //nice_time_t today = get_localtime();
  const auto& [ year, month, day, hour, minute, seconds ] = get_localtime();

  cout << "From current directory " << std::filesystem::current_path() << " writing log file to ../logs/" << endl;

  ofstream log_file("../logs/perflog-" + year + "-" + month + "-" + day + "+" + hour + ":" + minute + ":" + seconds + ".md");
  
  log_file << "# Performance test of the day" << endl << endl;
  log_file << "The goal of these measurements is to make sure that the time it takes to interpret the same sample does not change as I add features. Let's hope it never goes up!!" << endl << endl;
  log_file << "Note that the memory usage is measured for macOS only, it will not work properly on another OS." << endl << endl;
  log_file << "Exact time of creation: " << day << "/" << month << "/" << year << " (dd/mm/YYYY) at " << hour << ":" << minute << ":" << seconds << " Europe/Paris" << endl << endl;
  log_file << "Due to how the Parser works, it's quite difficult and problematic to try and measure the Lexer, Parser and Interpreter separately. Therefore, I measure the time the Parser took in total (from the beginning, therefore including lexical analysis). The interpreter measurements also include the time it took to analyse and parse the source code." << endl << endl;
  log_file << "|Feature|Time|Memory Usage|" << endl;
  log_file << "|-------|----|------------|" << endl;
  log_file << markdown_table_line("Lexer", lexer_measurements) << endl;
  log_file << markdown_table_line("Parser", parser_measurements) << endl;
  log_file << markdown_table_line("Interpreter", interpreter_measurements) << endl;
  log_file << "The lexer found " << number_of_tokens << " tokens for a source code of " << source_code.length() << " characters (" << source_code.length() * sizeof(char) << " bytes)" << endl << endl;
  log_file << "# Sample" << endl << endl;
  log_file << "This test was done my reading and interpreting this piece of valid code:" << endl << endl;
  log_file << "```" << endl;
  log_file << source_code << endl;
  log_file << "```" << endl;

  log_file.close();
  
  return 0;
}
