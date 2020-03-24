#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <cstdint>
#include <string>
#include "functor.h"

int64_t Timestamp();

void FlushCache();

void Benchmark(Functor* f, uint64_t num_elements, bool use_pointer,
               const std::string& description);

#endif  // BENCHMARK_H_
