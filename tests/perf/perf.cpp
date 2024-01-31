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
using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
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

void show_results(const string& name, const double time, const size_t memory_usage) {
  // If it took more than `treshold` milliseconds,
  // then show red result
  // otherwise show it in green
  const string str_time = std::to_string(time);
  const string answer = time > treshold ? get_failure(str_time) : get_success(str_time);
  cout << name << " individual performance test took " << answer << " ms and " << std::to_string(memory_usage) << " bytes of memory" << endl;
}

string pad(int number) {
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

int main() {
  cout << "Testing individual parts first..." << endl;
  const auto r1 = high_resolution_clock::now();
  const string source_code = read_entire_file("../tests/perf/source_code.txt");
  const auto r2 = high_resolution_clock::now();
  const double rt = get_milliseconds(r1, r2);

  cout << "Reading the source code took " << rt << " ms" << endl;
  cout << "The source code contains " << source_code.length() << " characters (" << source_code.length() * sizeof(char) << " bytes)" << endl;

  const auto lexer_musage1 = get_current_memory_usage();
  const auto l1 = high_resolution_clock::now();
  Lexer lexer(&source_code);
  lexer_rt tokens = lexer.generate_tokens();
  const auto l2 = high_resolution_clock::now();
  const auto lexer_musage2 = get_current_memory_usage();
  const double lt = get_milliseconds(l1, l2);
  const auto lm = lexer_musage2 - lexer_musage1;

  cout << "which produced " << tokens.size() << " tokens" << endl;

  const auto parser_musage1 = get_current_memory_usage();
  const auto p1 = high_resolution_clock::now();
  Parser parser(tokens);
  parser_rt ast = parser.parse();
  const auto p2 = high_resolution_clock::now();
  const auto parser_musage2 = get_current_memory_usage();
  const double pt = get_milliseconds(p1, p2);
  const auto pm = parser_musage2 - parser_musage1;

  const auto interpreter_musage1 = get_current_memory_usage();
  const auto i1 = high_resolution_clock::now();
  shared_ptr<Context> ctx = make_shared<Context>("<perf>");
  Interpreter::set_shared_ctx(ctx);
  Interpreter::visit(move(ast));
  const auto i2 = high_resolution_clock::now();
  const auto interpreter_musage2 = get_current_memory_usage();
  const double it = get_milliseconds(i1, i2);
  const auto im = interpreter_musage2 - interpreter_musage1;

  show_results("Lexer", lt, lm);
  show_results("Parser", pt, pm); // sometimes the memory usage of the lexer and the parser are exactly the same, and I've no idea why
  show_results("Interpreter", it, im);

  // Writing a log file with Markdown syntax.
  // I know the way I'm writing the file is kinda terrible,
  // but it really doesn't matter.

  //nice_time_t today = get_localtime();
  const auto& [ year, month, day, hour, minute, seconds ] = get_localtime();

  cout << "Current directory: " << std::filesystem::current_path() << endl;

  ofstream log_file("../logs/perflog-" + year + "-" + month + "-" + day + "+" + hour + ":" + minute + ":" + seconds + ".md");
  
  log_file << "# Performance test of the day" << endl << endl;
  log_file << "The goal of these measurements is to make sure that the time it takes to interpret the same sample does not change as I add features. Let's hope it never goes up!!" << endl << endl;
  log_file << "Note that the memory usage is measured for macOS only, it will not work properly on another OS." << endl << endl;
  log_file << "Exact time of creation: " << day << "/" << month << "/" << year << " (dd/mm/YYYY) at " << hour << ":" << minute << ":" << seconds << " Europe/Paris" << endl << endl;
  log_file << "|Feature|Time|Memory Usage|" << endl;
  log_file << "|-------|----|------------|" << endl;
  log_file << "|Lexer|" << std::to_string(lt) << " ms|" + std::to_string(lm) << " bytes, " << lm / 1024 <<  " kbi|" << endl;
  log_file << "|Parser|" << std::to_string(pt) << " ms|" + std::to_string(pm) << " bytes, " << pm / 1024 <<  " kbi|" << endl;
  log_file << "|Interpreter|" << std::to_string(it) << " ms|" + std::to_string(im) << " bytes, " << im / 1024 <<  " kbi|" << endl << endl;
  log_file << "The lexer found " << tokens.size() << " tokens for a source code of " << source_code.length() << " characters (" << source_code.length() * sizeof(char) << " bytes)" << endl << endl;
  log_file << "# Sample" << endl << endl;
  log_file << "This test was done my reading and interpreting this piece of valid code:" << endl << endl;
  log_file << "```" << endl;
  log_file << source_code << endl;
  log_file << "```" << endl;

  log_file.close();
  
  return 0;
}
