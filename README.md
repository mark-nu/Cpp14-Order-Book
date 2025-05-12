# Cpp14-Order-Book

A high-performance, C++14 limit-order book matching engine. This library processes streams of order events (ADD, MODIFY, CANCEL), enforces price–time priority, and outputs midquotes, trade executions, and full book snapshots. It can run as a standalone benchmarking tool or be embedded into larger trading systems.

---

## Table of Contents

- [Features](#features)
- [Components](#components)
- [Prerequisites](#prerequisites)
- [Building](#building)
  - [Standalone Binary](#standalone-binary)
  - [Tests](#tests)
- [Running](#running)
- [Testing](#testing)

---

## Features

- **Limit-order support**: Add, modify, and cancel buy/sell limit orders.
- **Automated matching**: Cross incoming orders against the opposite side, generating trades with price–time priority.
- **Midquote reporting**: Compute and print the midpoint between best bid and ask in real time.
- **Rolling trade aggregation**: Accumulate trades at the same price level before flushing.
- **Lightweight integration**: Core data structures and interfaces are easy to embed.
- **Benchmarking mode**: Measure processing time of input feed files across multiple trials.
- **Comprehensive test suite**: Unit and integration tests via Google Test.

---

## Components

### Common Types

Basic aliases for `OrderId`, `Quantity`, and `Price`, used throughout the engine.  
_Files: `utils/Common.h`_

### Order

Encapsulates a single order or trade message with four actions:

- **ADD** (`A,<id>,<side>,<qty>,<price>`)
- **CANCEL** (`X,<id>`)
- **MODIFY** (`M,<id>,<side>,<qty>,<price>`)
- **TRADE** (`T,<qty>,<price>`)  
  _Files: `utils/Order.h`, `utils/Order.cpp`_

### OrderBook

Maintains two sorted books (buy and sell) via:

- A global FIFO list for arrival-order tracking
- Price buckets mapping price levels to lists of orders
- Unordered maps for O(1) lookup/removal

Exposes:

- `addOrder()`, `modifyOrder()`, `cancelOrder()`
- `trade()` to accumulate and report executions  
  _Files: `src/OrderBook.h`, `src/OrderBook.cpp`_

### Matcher

Continuously matches best buy and sell orders until no crossable prices remain. Executes trades at the resting order’s price, updates quantities, and removes filled orders.  
_Files: `src/Matcher.h`, `src/Matcher.cpp`_

### FeedHandler

Orchestrates parsing and dispatching of messages:

1. Parses each input line into an `Order` object
2. Calls `addOrder`, `modifyOrder`, `cancelOrder`, or `trade`
3. Triggers matching on each ADD action  
   _Files: `src/FeedHandler.h`, `src/FeedHandler.cpp`_

### Reporter

Provides real-time output:

- **printMidQuote**: Displays midpoint of best bid/ask or “NAN” if one side is empty
- **printCurrentOrderBook**: Dumps active price levels and order quantities for both sides  
  _Files: `src/Reporter.h`, `src/Reporter.cpp`_

### Main Benchmarking Tool

`main.cpp` executable:

- Accepts one or more feed file paths as arguments
- Runs each file across multiple trials, measuring processing time
- Prints intermittent midquotes and snapshots to `stderr`
- Writes CSV-style timing results and averages to `build/output.txt`  
  _File: `src/main.cpp`_

---

## Prerequisites

- A C++14-compatible compiler (e.g., GCC ≥ 5.0 or Clang ≥ 3.4)
- [Google Test](https://github.com/google/googletest) for unit testing
- A POSIX shell or Make for build scripts

---

## Building

### Standalone Binary

```bash
git clone https://github.com/mark-nu/Cpp14-Order-Book.git
cd Cpp14-Order-Book
g++ -std=c++14 -O2 src/*.cpp utils/*.cpp -o order_book
```

### Tests

```bash
# Adjust include paths for your system’s gtest installation
g++ -std=c++14 -isystem /usr/include/gtest -pthread \
    tests/test.OrderBook.cpp src/*.cpp utils/*.cpp \
    -lgtest -lgtest_main -o order_book_tests
```

## Running

```bash
# Process one or more feed files
./order_book path/to/feed1.txt path/to/feed2.txt

# - build/output.txt   — CSV of timing results
# - stdout             — final book snapshots
# - stderr             — midquotes & intermittent snapshots
```

## Testing

```bash
./order_book_tests
```

### Covers:

- Order parsing for all action types
- OrderBook operations (add, modify, cancel)
- Matcher matching and aggregation
- Reporter midquote and book dumps
- Integration over a sample feed
