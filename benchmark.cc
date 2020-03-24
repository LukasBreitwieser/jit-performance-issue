#include "benchmark.h"

#include <unistd.h>
#include <chrono>
#include <iostream>
#include <random>
#include "functor.h"

int64_t Timestamp() {
  using std::chrono::milliseconds;
  using std::chrono::duration_cast;
  auto time = std::chrono::high_resolution_clock::now();
  auto since_epoch = time.time_since_epoch();
  auto millis = duration_cast<milliseconds>(since_epoch);
  return millis.count();
}

void FlushCache() {
  const uint64_t bigger_than_cachesize = 100 * 1024 * 1024;
  char* buffer = new char[bigger_than_cachesize];
  char r = rand();
  for (uint64_t i = 0; i < bigger_than_cachesize; i++) {
    buffer[i] = r + i;
  }
  delete buffer;
}

void Benchmark(Functor* f, uint64_t num_elements, bool use_pointer,
               const std::string& description) {
  std::vector<double> data;
  data.resize(num_elements);

  for (uint64_t i = 0; i < num_elements; ++i) {
    data[i] = i;
  }

  FlushCache();
  usleep(500);

  auto start = Timestamp();
#pragma omp parallel for
  for (uint64_t i = 0; i < num_elements; ++i) {
    for (uint64_t j = 0; j < 10; ++j) {
      if (use_pointer) {
        data[i] = (*f)(&data[i]);
      } else {
        data[i] = (*f)(data[i]);
      }
    }
  }
  std::cout << description << Timestamp() - start << std::endl;

  usleep(500);
  // do not optimize away
  volatile auto copy = *((char*)&data.back());
}
