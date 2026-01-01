
#include <benchmark/benchmark.h>

#include <string>
#include <map>
#include <array>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "./Formatter.h"
#include "./JSONFormatter.h"
#include "./stringify.h"


using std::string;
using std::map;
using std::array;
using namespace fmt::literals;

typedef std::map<std::string,std::string> osd_alert_list_t;
/// map osd id -> alert_list_t
typedef std::map<int, osd_alert_list_t> osd_alerts_t;


void dump(Formatter* f, const osd_alerts_t& alerts)
{
  for (auto& a : alerts) {
    string s0 = " osd: ";
    s0 += stringify(a.first);
    string s;
    for (auto& aa : a.second) {
      s = s0;
      s += " ";
      s += aa.first;
      s += ":";
      s += aa.second;
      f->dump_string("alert", s);
    }
  }
}

void dump_nv1(Formatter* f, const osd_alerts_t& alerts)
{
  for (const auto& a : alerts) {
    string s0 = " osd: ";
    s0 += stringify(a.first);
    string s;
    for (const auto& aa : a.second) {
      f->dump_named_fmt("alert", "{} {}:{}", s0, aa.first, aa.second);
      // s = s0;
      // s += " ";
      // s += aa.first;
      // s += ":";
      // s += aa.second;
      // f->dump_string("alert", s);
    }
  }
}


void dump_nv1_but_dumpStr(Formatter* f, const osd_alerts_t& alerts)
{
  for (const auto& a : alerts) {
    string s0 = " osd: ";
    s0 += stringify(a.first);
    string s;
    for (const auto& aa : a.second) {
      f->dump_string("alert", fmt::format("{} {}:{}"_cf, s0, aa.first, aa.second));
      // s = s0;
      // s += " ";
      // s += aa.first;
      // s += ":";
      // s += aa.second;
      // f->dump_string("alert", s);
    }
  }
}

void dump_nv2(Formatter* f, const osd_alerts_t& alerts)
{
  for (const auto& a : alerts) {
    string s0 = fmt::format(" osd: {}", a.first);
    //s0 += stringify(a.first);
    //string s;
    for (const auto& aa : a.second) {
      f->dump_named_fmt("alert", "{} {}:{}", s0, aa.first, aa.second);
      // s = s0;
      // s += " ";
      // s += aa.first;
      // s += ":";
      // s += aa.second;
      // f->dump_string("alert", s);
    }
  }
}

void dump_nv3(Formatter* f, const osd_alerts_t& alerts)
{
  for (const auto& a : alerts) {
    //string s0 = fmt::format(" osd: {}", a.first);
    //s0 += stringify(a.first);
    //string s;
    std::for_each(a.second.begin(), a.second.end(),
      [&](const auto& aa) {
        f->dump_named_fmt("alert", " osd: {} {}:{}", a.first, aa.first, aa.second);
      });
  }
}

void dump_nv3_but_dumpStr(Formatter* f, const osd_alerts_t& alerts)
{
  for (const auto& a : alerts) {
    for (const auto& aa : a.second) {
      f->dump_string("alert", fmt::format(" osd: {} {}:{}", a.first, aa.first, aa.second));
    }
  }
}


osd_alerts_t create_test_case_alerts()
{
  osd_alerts_t alerts;
  for (int i = 0; i < 1000; ++i) {
    osd_alert_list_t& alist = alerts[i];
    for (int j = 0; j < 16; ++j) {
      string key = "alert_type_" + stringify(j);
      string val = "alert_value_msgmsgmsgmsg_" + stringify(j);
      alist[key] = val;
    }
  }
  return alerts;
}


void test_sameness()
{
  osd_alerts_t alerts = create_test_case_alerts();
  //std::vector<int> ks{11,21,31,41,51,61,71,81,91,101};

  //for (int k : ks) {
    Formatter* f0 = Formatter::create("json-pretty");
    dump(f0, alerts);
    std::stringstream ss0;
    f0->flush(ss0);
    delete f0;

    Formatter* f1 = Formatter::create("json-pretty");
    dump_nv1(f1, alerts);
    std::stringstream ss1;
    f1->flush(ss1);
    delete f1;

    assert(ss0.str() == ss1.str());

    Formatter* f2 = Formatter::create("json-pretty");
    dump_nv2(f2, alerts);
    std::stringstream ss2;
    f2->flush(ss2);
    delete f2;
    assert(ss0.str() == ss2.str());


    Formatter* f3 = Formatter::create("json-pretty");
    dump_nv3(f3, alerts);
    std::stringstream ss3;
    f3->flush(ss3);
    delete f3;
    assert(ss0.str() == ss3.str());

    Formatter* f3s = Formatter::create("json-pretty");
    dump_nv3_but_dumpStr(f3s, alerts);
    std::stringstream ss3s;
    f3s->flush(ss3s);
    delete f3s;
    assert(ss0.str() == ss3s.str());
  //}
}





std::byte buffer[10 * 1024 * 1024];


static void alerts_orig(benchmark::State& state)
{
  const auto o1 = create_test_case_alerts();
  Formatter* f = Formatter::create("json-pretty");
  int k{0};
  for (auto _ : state) {
    k = (k + 1) % 100;
    dump(f, o1);
    auto stam = f->test_extract(k);
    benchmark::DoNotOptimize(stam);
    f->reset();
  }
}
// Register the function as a benchmark
BENCHMARK(alerts_orig);

static void alerts_nv1(benchmark::State& state)
{
  const auto o1 = create_test_case_alerts();
  Formatter* f = Formatter::create("json-pretty");
  int k{0};
  for (auto _ : state) {
    k = (k + 1) % 100;
    dump_nv1(f, o1);
    auto stam = f->test_extract(k);
    benchmark::DoNotOptimize(stam);
    f->reset();
  }
}
// Register the function as a benchmark
BENCHMARK(alerts_nv1);


static void alerts_nv2(benchmark::State& state)
{
  const auto o1 = create_test_case_alerts();
  Formatter* f = Formatter::create("json-pretty");
  int k{0};
  for (auto _ : state) {
    k = (k + 1) % 100;
    dump_nv2(f, o1);
    auto stam = f->test_extract(k);
    benchmark::DoNotOptimize(stam);
    f->reset();
  }
}
// Register the function as a benchmark
BENCHMARK(alerts_nv2);


static void alerts_nv3(benchmark::State& state)
{
  const auto o1 = create_test_case_alerts();
  Formatter* f = Formatter::create("json-pretty");
  int k{0};
  for (auto _ : state) {
    k = (k + 1) % 100;
    dump_nv3(f, o1);
    auto stam = f->test_extract(k);
    benchmark::DoNotOptimize(stam);
    f->reset();
  }
}
// Register the function as a benchmark
BENCHMARK(alerts_nv3);



static void alerts_nv1_butDumpString(benchmark::State& state)
{
  const auto o1 = create_test_case_alerts();
  Formatter* f = Formatter::create("json-pretty");
  int k{0};
  for (auto _ : state) {
    k = (k + 1) % 100;
    dump_nv1_but_dumpStr(f, o1);
    auto stam = f->test_extract(k);
    benchmark::DoNotOptimize(stam);
    f->reset();
  }
}
// Register the function as a benchmark
BENCHMARK(alerts_nv1_butDumpString);


static void alerts_nv3_butDumpString(benchmark::State& state)
{
  const auto o1 = create_test_case_alerts();
  Formatter* f = Formatter::create("json-pretty");
  int k{0};
  for (auto _ : state) {
    k = (k + 1) % 100;
    dump_nv3_but_dumpStr(f, o1);
    auto stam = f->test_extract(k);
    benchmark::DoNotOptimize(stam);
    f->reset();
  }
}
// Register the function as a benchmark
BENCHMARK(alerts_nv3_butDumpString);


// static void test_nv1(benchmark::State& state)
// {
//   const auto o1 = create_test_case_alerts();
//   Formatter* f = Formatter::create("json-pretty");
//   int k{0};
//   for (auto _ : state) {
//     k = (k + 1) % 100;
//     dump_nv1(f, o1);
//     auto stam = f->test_extract(k);
//     benchmark::DoNotOptimize(stam);
//     f->reset();
//   }
// }
// // Register the function as a benchmark
// BENCHMARK(alerts_nv1);


#if 1

BENCHMARK_MAIN();

#else
int main()
{
  test_sameness();

}


#endif




