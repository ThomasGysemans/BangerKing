#pragma once

#include <iostream>
#include <string>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// https://stackoverflow.com/a/22387757/14522489

class Performance {
  unsigned int iterations;

  inline static const unsigned int threshold = 1;
  inline static const string ANSI_RED = "\e[0;31m";
  inline static const string ANSI_GREEN = "\e[0;32m";
  inline static const string ANSI_RESET = "\e[0m";

  public:
    Performance(unsigned int i): iterations(i) {}

  protected:
    virtual void test() const = 0;

  private:
    const double measure_time() const {
      auto t1 = high_resolution_clock::now();
      test();
      auto t2 = high_resolution_clock::now();
      duration<double, std::milli> ms_double = t2 - t1;
      return ms_double.count();
    }
  
  public:
    double run() const {
      if (iterations == 0) {
        return 0.0;
      }
      double sum = 0.0;
      for (int i = 0; i < iterations; ++i) {
        double t = measure_time();
        sum += t;
      }
      return sum / iterations;
    }

    void run_and_display(const string& title) const {
      double average = run();
      cout << title;
      if (average < threshold) {
        cout << get_success(std::to_string(average)) << " ms.";
      } else {
        cout << get_failure(std::to_string(average)) << " ms.";
      }
      cout << endl;
    }

    static string get_success(const string& message) {
      return ANSI_GREEN + message + ANSI_RESET;
    }

    static string get_failure(const string& message) {
      return ANSI_RED + message + ANSI_RESET;
    }
};
