cmake_minimum_required(VERSION 3.24)
project(OrderBook LANGUAGES CXX)

# ———————— C++ STANDARD & FLAGS ————————
if(DEFINED CXX_STANDARD)
  set(CMAKE_CXX_STANDARD ${CXX_STANDARD})
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

# (Optional) combine any extra flags you need
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${OPT_FLAGS} ${DEBUG_FLAGS} ${SAN_FLAGS} ${EXTRA_OPTIONS}"
)
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")

# ———————— CORE LIBRARY ————————
# Gather all .cpp under src/ and utils/, but exclude the app's main.cpp
file(GLOB CORE_SOURCES
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/utils/*.cpp"
)
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_library(orderbook_core STATIC ${CORE_SOURCES})
target_include_directories(orderbook_core
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/utils"
)

# ———————— APPLICATION EXECUTABLE ————————
add_executable(orderbook_app
  "${CMAKE_SOURCE_DIR}/src/main.cpp"
)
target_link_libraries(orderbook_app PRIVATE orderbook_core)

# ———————— GOOGLETEST SETUP ————————
include(FetchContent)
enable_testing()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ———————— UNIT TEST TARGET ————————
add_executable(test_unit
  "${CMAKE_SOURCE_DIR}/tests/test.unit.OrderBook.cpp"
)
target_link_libraries(test_unit
  PRIVATE
    orderbook_core
    gtest_main
)
target_include_directories(test_unit
  PRIVATE
    ${gtest_SOURCE_DIR}/include
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/utils"
)
add_test(NAME test_unit COMMAND test_unit)
set_tests_properties(test_unit
  PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ———————— INTEGRATION TEST TARGET ————————
add_executable(test_integration
  "${CMAKE_SOURCE_DIR}/tests/test.integration.OrderBook.cpp"
)
target_link_libraries(test_integration
  PRIVATE
    orderbook_core
    gtest_main
)
target_include_directories(test_integration
  PRIVATE
    ${gtest_SOURCE_DIR}/include
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/utils"
)
# Make the source-root path available to your test code:
target_compile_definitions(test_integration
  PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
add_test(NAME test_integration COMMAND test_integration)
set_tests_properties(test_integration
  PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
