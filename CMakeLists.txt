cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(named_fmt_perf CXX)

#dd_compile_options(-std=c++20)

#set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -O3 -Wall -fomit-frame-pointer -I/home/rfriedma/apps/fmt/include -L/home/rfriedma/apps/fmt /home/rfriedma/apps/fmt/libfmt.a")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -g -O1 -Wall -fomit-frame-pointer ")
find_package(benchmark REQUIRED)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        40626af88bd7df9a5fb80be7b25ac85b122d6c21) # 11.2.0
FetchContent_MakeAvailable(fmt)


set(SOURCE main.cc named.cc named.h Formatter.cc JSONFormatter.cc JSONFormatter.h StackStringStream.h Formatter.h escape.cc escape.h)


add_executable(${PROJECT_NAME} ${SOURCE})
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# fmt 10: target_link_libraries(named_fmt_perf benchmark::benchmark /home/rfriedma/apps/fmt/libfmt.a)
target_link_libraries(named_fmt_perf benchmark::benchmark fmt::fmt)


