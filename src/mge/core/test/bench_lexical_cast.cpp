// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/lexical_cast.hpp"

#include <sstream>

static void bench_lexical_cast_noop(benchmark::State& state) {
    std::string s1("horst");
    std::string s2;
  while (state.KeepRunning()) {
      s2 = mge::lexical_cast<decltype(s2)>(s1);
  }
}
static void bench_lexical_cast_string_to_int(benchmark::State& state) {
    std::string s1("123");
    int s2;
  while (state.KeepRunning()) {
      s2 = mge::lexical_cast<decltype(s2)>(s1);
  }
}

static void bench_lexical_cast_int_to_string(benchmark::State& state) {
    int s1 = 123;
    std::string s2;
  while (state.KeepRunning()) {
      s2 = mge::lexical_cast<decltype(s2)>(s1);
  }
}

static void bench_int_to_string_stringstream(benchmark::State& state) {
    int s1 = 123;
    while (state.KeepRunning()) {
        std::stringstream ss;
        ss << s1;
        std::string s2(ss.str());
    }
}

BENCHMARK(bench_lexical_cast_noop);
BENCHMARK(bench_lexical_cast_string_to_int);
BENCHMARK(bench_lexical_cast_int_to_string);
BENCHMARK(bench_int_to_string_stringstream);
