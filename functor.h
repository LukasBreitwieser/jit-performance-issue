#ifndef FUNCTOR_H_
#define FUNCTOR_H_

struct Functor {
  virtual double operator()(double* dp) = 0;
  virtual double operator()(double d) = 0;
};

#endif  // FUNCTOR_H_
