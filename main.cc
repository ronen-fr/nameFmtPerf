
#include <benchmark/benchmark.h>

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include "./Formatter.h"
#include "./JSONFormatter.h"
#include "./stringify.h"


using std::string;
using std::map;























#if 1


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
      f->dump_string("alert", fmt::format("{} {}:{}", s0, aa.first, aa.second));
      // s = s0;
      // s += " ";
      // s += aa.first;
      // s += ":";
      // s += aa.second;
      // f->dump_string("alert", s);
    }
  }
}

osd_alerts_t create_test_case_alerts()
{
  osd_alerts_t alerts;
  for (int i = 0; i < 1000; ++i) {
    osd_alert_list_t& alist = alerts[i];
    for (int j = 0; j < 10; ++j) {
      string key = "alert_type_" + stringify(j);
      string val = "alert_value_" + stringify(j);
      alist[key] = val;
    }
  }
  return alerts;
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


BENCHMARK_MAIN();

#endif


#if 0

int main()
{
  osd_alerts_t alerts;

  Formatter* f = Formatter::create("json-pretty");

  f->output_header();
  dump(f, alerts);
  f->output_footer();

  std::cout << "Formatted output:\n";
  f->flush(std::cout);

  delete f;

  return 0;
}


#endif




