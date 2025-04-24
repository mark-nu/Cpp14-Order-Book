#!/usr/bin/env bash
set -euo pipefail

# location of brewed gtest
GTEST_PREFIX="$(brew --prefix googletest)"

# find every .cpp under the project (including tests/) except main.cpp
SRC_AND_TESTS=$(find . -type f -name '*.cpp' \
    ! -name 'main.cpp' \
    ! -path './build/*')

clang++ -std=c++14 -g \
    -I"${GTEST_PREFIX}/include" \
    ${SRC_AND_TESTS} \
    -L"${GTEST_PREFIX}/lib" -lgtest -lgtest_main -pthread \
    -o build/Cpp14-Order-Book_test
