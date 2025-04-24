#!/usr/bin/env bash
set -euo pipefail

# location of brewed gtest
GTEST_PREFIX="$(brew --prefix googletest)"

# find all .cpp in the root (and in any subdirs), except main.cpp
SRC_FILES=$(
  find . -type f -name '*.cpp' \
       ! -name 'main.cpp' \
       ! -path './build/*' \
       ! -path './tests/*'
)

clang++ -std=c++14 -g \
  -I"${GTEST_PREFIX}/include" \
  $SRC_FILES \
  tests/unit/test.OrderBook.cpp \
  -L"${GTEST_PREFIX}/lib" -lgtest -lgtest_main -pthread \
  -o build/Cpp14-Order-Book_test
