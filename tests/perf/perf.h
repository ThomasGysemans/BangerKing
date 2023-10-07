#ifndef PERF_H
#define PERF_H

#include <iostream>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// https://stackoverflow.com/a/22387757/14522489

class Performance {
  unsigned int iterations;

  public:
    Performance(unsigned int i): iterations(i) {}
  
  protected:
    virtual void test() const {}

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
};

#endif