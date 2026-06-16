# Exchange Simulator

A C++ implementation of a simplified electronic stock exchange built from first principles. The project models the core components of modern financial markets, including a limit order book, matching engine, trader accounts, trade settlement, and exchange orchestration.

The objective of this project is to gain a deeper understanding of exchange architecture, market microstructure, and the design of high-performance trading systems.

---

## Features

### Implemented

* Limit buy and sell orders
* Price-time priority matching
* Limit order book
* Trade generation
* Partial order fills
* Multi-level order matching
* Exchange abstraction layer
* Trader accounts
* Order registry
* Trade history
* Cash balance tracking
* Position tracking
* Trade settlement

### Planned

* Order status tracking
* Order cancellation
* Market orders
* Risk management checks
* Multi-symbol support
* Market simulation
* Performance benchmarking
* Analytics and reporting

---

## Architecture

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

### Exchange

The `Exchange` acts as the central coordinator of the system.

Responsibilities:

* Register traders
* Accept and validate orders
* Maintain trader and order registries
* Route orders to the matching engine
* Settle completed trades
* Track account balances and positions

### Matching Engine

The `MatchingEngine` is responsible for matching incoming orders against resting liquidity in the order book.

Matching follows:

1. Price Priority
2. Time Priority

Whenever compatible orders are found, the engine generates trades.

### Order Book

The order book stores all active orders.

Buy orders are prioritized by:

* Higher price
* Earlier timestamp

Sell orders are prioritized by:

* Lower price
* Earlier timestamp

### Traders

Each trader maintains:

* Trader ID
* Cash balance
* Net position

All traders begin with a configurable initial cash balance.

### Trade Settlement

After every trade:

#### Buyer

* Cash decreases by `quantity × price`
* Position increases by `quantity`

#### Seller

* Cash increases by `quantity × price`
* Position decreases by `quantity`

---

## Matching Example

Resting order:

```text
BUY 100 @ 500
```

Incoming order:

```text
SELL 50 @ 480
```

Since:

```text
500 >= 480
```

a trade occurs.

Execution:

```text
50 @ 500
```

The trade executes at the price of the resting order, which is consistent with the behavior of most modern exchanges.

---

## Project Structure

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

## Example Output

```text
Trades
1 1 4 50 500
2 1 5 50 500

Remaining Buys
2 80 495
3 60 490

Remaining Sells
5 20 500
6 120 505

Trader 1
950000 100

Trader 2
1050000 -100
```

---

## Build

Using g++:

```bash
g++ src/*.cpp -Iinclude -std=c++17 -o exchange
```

Using CMake:

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## Run

```bash
./exchange
```

---

## Current Status

The project currently supports end-to-end order processing:

1. Traders submit orders to the exchange.
2. The exchange validates and records orders.
3. Orders are forwarded to the matching engine.
4. Trades are generated using price-time priority.
5. Trade settlement updates trader balances and positions.
6. Remaining liquidity is maintained in the order book.

The next development milestone is implementing order status tracking and order cancellation.