/**
 * Copyright (c) 2018-2019 Mingxin Wang. All rights reserved.
 */

#include <iostream>

#include "../../main/p0957/proxy.h"
#include "../../main/p0957/mock/proxy_fc_impl.h"

// Indicates the life cycle of an object
class Indicator {
 public:
  Indicator() : ID(++tot_id_) {
    printf("Indicator %d: Default Construct\n", ID);
  }

  Indicator(const Indicator&) : ID(++tot_id_) {
    printf("Indicator %d: Copy Construct\n", ID);
  }

  Indicator(Indicator&&) : ID(++tot_id_) {
    printf("Indicator %d: Move Construct\n", ID);
  }

  ~Indicator() {
    printf("Indicator %d: Destruct\n", ID);
  }

  void hint() { printf("%d\n", ID); }

 private:
  static int tot_id_;
  const int ID;
};

int Indicator::tot_id_ = 0;

// Has facade FA
class FaDemo1 {
 public:
  explicit FaDemo1(int num) : num_(num) {}

  int fun_a_0() {
    puts("FaDemo1: fun_a_0 called");
    printf("Check: %d\n\n", num_);
    return -1;
  }

  int fun_a_1(double x) {
    puts("FaDemo1: fun_a_0 called");
    printf("Input: %f\n", x);
    printf("Check: %d\n\n", num_);
    return static_cast<int>(-x);
  }

 private:
  int num_;
  Indicator indicator_;
};

// Has facade FB
class FbDemo1 {
 public:
  explicit FbDemo1(int num) : num_(num) {}

  void fun_b(std::p0957::value_proxy<FA> p) {
    puts("FbDemo1: fun_b called");
    p.fun_a_0();
    std::cout << p.fun_a_1(5) << std::endl;
    printf("Check: %d\n\n", num_);
  }

 private:
  int num_;
  Indicator indicator_;
};

// Has facade FC
class FcDemo1 {
 public:
  explicit FcDemo1(int num) : num_(num) {}

  FcDemo1(const FcDemo1& rhs) : num_(rhs.num_) {
    puts("FcDemo1: copy ctor called");
    printf("Check: %d\n\n", num_);
  }

  int fun_a_0() {
    puts("FcDemo1: fun_a_0 called");
    printf("Check: %d\n\n", num_);
    return 1;
  }

  int fun_a_1(double var) {
    puts("FcDemo1: fun_a_1 called");
    printf("Input var: %f\n", var);
    printf("Check: %d\n\n", num_);
    return 5;
  }

  static void fun_b(std::p0957::value_proxy<FA> p) {
    puts("FcDemo1: static fun_b called");
    p.fun_a_0();
    std::cout << p.fun_a_1(5) << std::endl;
  }

  void fun_c() {
    puts("FcDemo1: fun_c called");
    printf("Check: %d\n\n", num_);
  }

 private:
  int num_;
  Indicator indicator_;
};

int main() {
  std::p0957::copyable_value_proxy<FC> p1(FcDemo1{8});

  // FcDemo1::fun_a_0() is called.
  p1.fun_a_0();

  p1.emplace<FcDemo1>(7);

  // FcDemo1::fun_a_1(double) is called.
  p1.fun_a_1(1.5);

  // FcDemo1::fun_b(value_proxy<FA>) is called
  p1.fun_b(FaDemo1{123});

  // FcDemo1::fun_c() is called.
  p1.fun_c();

  p1.emplace<FcDemo1>(23);

  p1.fun_a_0();

  auto p2 = p1;

  p1.reset();

  puts(p1.has_value() ? "Valid state" : "Invalid State");

  auto value = FcDemo1{10};
  std::p0957::reference_proxy<FB> p3(value);

  // FcDemo1::fun_b(value_proxy<FA>) is called
  p3.fun_b(FaDemo1{456});
}
