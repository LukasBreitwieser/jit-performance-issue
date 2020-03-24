#include <chrono>
#include <iostream>
#include "TInterpreter.h"
#include "TSystem.h"

#include "benchmark.h"
#include "functor.h"

struct CompiledFunctor : public Functor {
  double operator()(double* dp) { return 1.1 * (*dp); }
  double operator()(double d) { return 1.1 * d; }
};

int main() {
  // gInterpreter->ProcessLineSynch(".O 3");
  gInterpreter->ProcessLineSynch("R__LOAD_LIBRARY(libfunctor_dictDict)");
  gInterpreter->ProcessLineSynch(R"EOF(
    #pragma cling optimize(3);
    struct JitFunctor : public Functor { 
      double operator()(R__CLING_PTRCHECK(off) double* dp) { 
      //double operator()(double* dp) { 
        return 1.1 * (*dp); 
      }
      
      double operator()(double d) { return 1.1 * d; }
    };)EOF");

  //auto* jit = (Functor*)gInterpreter->Calc("new JitFunctor();");
  auto* jit = (Functor*)gInterpreter->Calc("#pragma cling optimize(3)\n new JitFunctor();");
  uint64_t num_elements = 1e8;
  Benchmark(jit, num_elements, true, "jit (pointer)      ");
  Benchmark(jit, num_elements, false, "jit (double)       ");

  auto* compiled = new CompiledFunctor();
  Benchmark(compiled, num_elements, true, "compiled (pointer) ");
  Benchmark(compiled, num_elements, false, "compiled (double)  ");

  delete jit;
  delete compiled;
  return 0;
}
