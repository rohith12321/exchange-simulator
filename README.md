# Exchange Simulator

A C++ implementation of a concurrent multi-symbol electronic exchange built from first principles.

## Final Target Architecture

```text
                 Router
                    │

      --------------------------
      │            │           │

      ▼            ▼           ▼

    AAPL         GOOG        MSFT

 +---------+   +---------+  +---------+
 | Order   |   | Order   |  | Order   |
 | Queue   |   | Queue   |  | Queue   |
 +---------+   +---------+  +---------+

      │            │           │

      ▼            ▼           ▼

 +---------+   +---------+  +---------+
 |Matching |   |Matching |  |Matching |
 | Thread  |   | Thread  |  | Thread  |
 +---------+   +---------+  +---------+

      │            │           │

      └────────────┼───────────┘
                   │
                   ▼

            +-------------+
            | Trade Queue |
            +-------------+

                   │
                   ▼

            +-------------+
            | Logger      |
            | Thread      |
            +-------------+
```

The objective is to build a concurrent exchange capable of handling multiple traders, multiple instruments, and independent matching engines while applying operating systems concepts such as threads, synchronization, producer-consumer pipelines, and thread-safe communication.

---

# Current Architecture

```text
Exchange
│
├── MatchingEngine
│   │
│   ├── OrderBook
│   └── Trade History
│
├── Trader Registry
│
└── Order Registry
```

Current implementation:

* Limit Orders
* Market Orders
* Price-Time Priority
* Partial Fills
* Trade Generation
* Trade Settlement
* Trader Accounts
* Cash Tracking
* Position Tracking
* Order Status Tracking
* Order Cancellation
* Risk Management

---

# Matching Logic

Matching follows:

1. Price Priority
2. Time Priority

Buy-side priority:

```text
Higher Price
      ↓
Earlier Timestamp
```

Sell-side priority:

```text
Lower Price
      ↓
Earlier Timestamp
```

Example:

```text
BUY 100 @ 500
```

matched with

```text
SELL 50 @ 480
```

produces

```text
50 @ 500
```

using the resting order's price.

---

# Present Data Flow

```text
Trader
   │
   ▼

Exchange
   │
   ▼

Matching Engine
   │
   ▼

Order Book
   │
   ▼

Trades
   │
   ▼

Settlement
```

Everything currently runs on a single thread.

---

# Next Milestone

The next step is introducing concurrency without changing matching behaviour.

## Target Data Flow

```text
Trader Threads
       │
       ▼

+------------------+
|   Order Queue    |
+------------------+

       │
       ▼

+------------------+
| Matching Thread  |
+------------------+

       │
       ▼

+------------------+
|   Trade Queue    |
+------------------+

       │
       ▼

+------------------+
|  Logger Thread   |
+------------------+
```

Design principles:

* Traders never touch the order book directly.
* Only the matching thread owns the order book.
* Communication occurs through thread-safe queues.
* Matching remains deterministic.
* Price-time priority remains unchanged.

---

# Planned Components

## Phase 1 — Exchange Core

Completed:

* Order Book
* Matching Engine
* Market Orders
* Limit Orders
* Trade Settlement
* Risk Management
* Order Cancellation
* Order Status Tracking

## Phase 2 — Concurrency

Planned:

* ThreadSafeQueue<Order>
* ThreadSafeQueue<Trade>
* Matching Thread
* Logger Thread
* Trader Threads
* Mutexes
* Condition Variables
* Graceful Thread Shutdown

## Phase 3 — Multi-Symbol Exchange

Planned:

* Symbol Router
* Multiple Order Books
* Multiple Matching Threads
* Independent Instrument Processing

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
│   ├── trade.hpp
│   └── trader.hpp
│
├── src/
│   ├── exchange.cpp
│   ├── matchingengine.cpp
│   ├── order.cpp
│   ├── orderbook.cpp
│   ├── trade.cpp
│   ├── trader.cpp
│   └── main.cpp
│
├── benchmarks/
├── docs/
├── tests/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

# Build

```bash
g++ src/*.cpp -Iinclude -std=c++17 -o exchange
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
