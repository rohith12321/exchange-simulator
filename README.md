# Concurrent Multi-Symbol Exchange Simulator

A concurrent electronic exchange simulator implemented in modern C++. The project models the core components of a modern stock exchange, including order books, matching engines, trade settlement, trader accounts, and concurrent order processing.

The primary objective of this project was to understand how electronic exchanges work while gaining practical experience with multithreading, synchronization, producer-consumer systems, and concurrent systems programming.

---

# Features

### Trading Engine

* Limit Orders
* Market Orders
* Price-Time Priority Matching
* Partial Fills
* Trade Generation
* Trade Settlement
* Order Cancellation
* Order Status Tracking
* Trader Accounts
* Cash & Position Tracking
* Basic Risk Validation
* Multi-Symbol Trading

### Concurrency

* Independent matching thread for every symbol
* Thread-safe producer-consumer queues
* Thread synchronization using mutexes and condition variables
* Safe concurrent trader account updates
* Graceful worker thread shutdown

---

# System Architecture

Each symbol owns an independent order queue, matching engine, and matching thread.

```text
                          Exchange
                              │
        ┌───────────────┬───────────────┬───────────────┐
        │               │               │
        ▼               ▼               ▼
      AAPL            GOOG            MSFT
        │               │               │
+---------------+ +---------------+ +---------------+
| ThreadSafe    | | ThreadSafe    | | ThreadSafe    |
| Order Queue   | | Order Queue   | | Order Queue   |
+---------------+ +---------------+ +---------------+
        │               │               │
        ▼               ▼               ▼
+---------------+ +---------------+ +---------------+
| Matching      | | Matching      | | Matching      |
| Thread        | | Thread        | | Thread        |
+---------------+ +---------------+ +---------------+
        │               │               │
        ▼               ▼               ▼
+---------------+ +---------------+ +---------------+
| Matching      | | Matching      | | Matching      |
| Engine        | | Engine        | | Engine        |
+---------------+ +---------------+ +---------------+
        │
        ▼
Trade Settlement
```

Orders are submitted by a single producer thread while each symbol is processed concurrently by its own matching thread.

---

# Major Components

## Exchange

The `Exchange` coordinates the entire system.

Responsibilities include:

* Managing trader accounts
* Validating incoming orders
* Maintaining the global order registry
* Routing orders to the appropriate symbol
* Settling completed trades
* Managing worker thread lifecycle

---

## Matching Engine

Each symbol owns its own `MatchingEngine`.

Responsibilities include:

* Processing incoming orders
* Matching against the opposite side of the order book
* Generating trades
* Maintaining trade history

Since every symbol has its own matching thread, different symbols can be processed concurrently.

---

## Order Book

The order book maintains separate buy and sell books using ordered containers.

Priority Rules

### Buy Orders

```text
Higher Price
      ↓
Earlier Timestamp
      ↓
Lower Order ID
```

### Sell Orders

```text
Lower Price
      ↓
Earlier Timestamp
      ↓
Lower Order ID
```

This guarantees deterministic price-time priority.

---

## Trader

Each trader stores

* Cash Balance
* Position

Trader state is protected internally using a mutex, allowing multiple trader accounts to be updated safely without introducing a global lock.

---

## ThreadSafeQueue

Communication between the producer and matching threads occurs through a custom thread-safe queue.

The queue uses

* `std::mutex`
* `std::condition_variable`

to implement a producer-consumer pipeline.

Features:

* Blocking pop
* Thread-safe push/pop
* Graceful shutdown
* Wake-up notification for worker threads

---

# Matching Algorithm

Incoming orders are matched against the opposite side of the order book.

Priority is determined by

1. Price
2. Timestamp
3. Order ID

Trades execute using the **resting order's price**.

Example

```text
Resting SELL
50 @ 480

Incoming BUY
100 @ 500

↓

Trade Executed

50 @ 480
```

Remaining quantity is either matched further or inserted into the order book.

---

# Order Lifecycle

```text
Trader

↓

Exchange

↓

Risk Validation

↓

Order Registry

↓

Symbol Queue

↓

Matching Thread

↓

Matching Engine

↓

Trade Generation

↓

Trade Settlement

↓

Order Status Update
```

---

# Synchronization

The exchange uses several synchronization primitives.

### Mutexes

Protect

* Order Registry
* Trader State
* ThreadSafeQueue

### Condition Variables

Used by the producer-consumer queues to

* Suspend idle matching threads
* Wake workers when new orders arrive
* Wait until all submitted orders have completed processing

---

# Project Structure

```text
exchange-simulator/
│
├── include/
│   ├── exchange.hpp
│   ├── matchingengine.hpp
│   ├── order.hpp
│   ├── orderbook.hpp
│   ├── symbolengine.hpp
│   ├── threadsafequeue.hpp
│   ├── trade.hpp
│   └── trader.hpp
│
├── src/
│   ├── exchange.cpp
│   ├── matchingengine.cpp
│   ├── order.cpp
│   ├── orderbook.cpp
│   ├── trader.cpp
│   └── trade.cpp
│
├── tests/
│   └── benchmark.cpp
│
├── README.md
├── CMakeLists.txt
└── .gitignore
```

---

# Benchmark Results

Benchmarks were executed on a machine with **16 hardware threads**.

## Benchmark 1 — Continuous Matching

Orders alternate between BUY and SELL at the same price, ensuring every incoming order immediately finds a matching counterpart.

```
Orders Submitted : 10,000,000
Trades Executed  : 5,000,000

Orders / Second : ~449,000
Trades / Second : ~225,000
```

This benchmark measures the maximum sustained throughput of the matching engine under a continuously matching workload.

---

## Benchmark 2 — Randomized Market Simulation

Random workload consisting of

* 100 Traders
* Random Symbols
* Random Buy/Sell Orders
* Random Prices
* Random Quantities

Representative Result

```
Orders Generated : 1,000,000
Orders Accepted  : 1,000,000
Orders Rejected  : 0
Trades Executed  : 773,613

Orders / Second : ~362,000
Trades / Second : ~280,000
```

Unlike the first benchmark, this workload allows the order books to evolve naturally, producing a more realistic distribution of resting and matched orders.

---

# Key Learnings

This project provided hands-on experience with

* Concurrent systems programming
* Producer-consumer architectures
* Thread synchronization
* Mutexes and condition variables
* Thread-safe data structures
* Order book implementation
* Matching engine design
* Trade settlement
* Performance benchmarking
* Scalability analysis

---

# Possible Future Improvements

* Lock-free queues
* Memory pools for orders and trades
* Time-in-force policies
* Order modification
* Iceberg orders
* Persistent trade logging
* Networked clients
* FIX protocol support

---

# Build

```bash
g++ src/*.cpp -Iinclude -std=c++17 -pthread -o exchange
```

or

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

# Run

```bash
./exchange
```
