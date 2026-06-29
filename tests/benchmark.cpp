#include <chrono>
#include <iomanip>
#include <iostream>

#include "../include/exchange.hpp"

using namespace std;
using Clock = chrono::steady_clock;

constexpr int NUM_ORDERS = 1'000'000;
constexpr long long INITIAL_CASH = 1'000'000'000'000'000LL;

int main(){

    Exchange exchange;

    exchange.addTrader(1, INITIAL_CASH);
    exchange.addTrader(2, INITIAL_CASH);

    long long orderId = 1;
    long long timestamp = 1;

    cout << "=============================================\n";
    cout << " Concurrent Exchange Throughput Benchmark\n";
    cout << "=============================================\n\n";

    cout << "Submitting "
         << NUM_ORDERS
         << " orders...\n";

    auto start = Clock::now();

    for(int i = 0; i < NUM_ORDERS / 2; i++){

        Symbol symbol =
            static_cast<Symbol>(i % 5);

        exchange.submitOrder(
            Order(
                orderId++,
                1,
                symbol,
                Side::BUY,
                OrderType::LIMIT,
                1,
                500,
                timestamp++
            )
        );

        exchange.submitOrder(
            Order(
                orderId++,
                2,
                symbol,
                Side::SELL,
                OrderType::LIMIT,
                1,
                500,
                timestamp++
            )
        );
    }

    exchange.waitUntilIdle();

    auto end = Clock::now();

    double elapsed =
        chrono::duration<double>(end - start).count();

    long long totalTrades = 0;

    for(Symbol symbol :
    {
        Symbol::AAPL,
        Symbol::GOOG,
        Symbol::MSFT,
        Symbol::NVDA,
        Symbol::TSLA
    }){
        totalTrades +=
            exchange
                .getMatchingEngine(symbol)
                .trades
                .size();
    }

    cout << fixed << setprecision(3);

    cout << "\n============= Results =============\n\n";

    cout << "Orders Submitted : "
         << NUM_ORDERS
         << '\n';

    cout << "Trades Executed  : "
         << totalTrades
         << '\n';

    cout << "Elapsed Time     : "
         << elapsed
         << " s\n";

    cout << "Orders / Second  : "
         << static_cast<long long>(
                NUM_ORDERS / elapsed
            )
         << '\n';

    cout << "Trades / Second  : "
         << static_cast<long long>(
                totalTrades / elapsed
            )
         << '\n';

    cout << "\nPer Symbol Trades\n";
    cout << "-----------------\n";

    cout << "AAPL : "
         << exchange.getMatchingEngine(Symbol::AAPL).trades.size()
         << '\n';

    cout << "GOOG : "
         << exchange.getMatchingEngine(Symbol::GOOG).trades.size()
         << '\n';

    cout << "MSFT : "
         << exchange.getMatchingEngine(Symbol::MSFT).trades.size()
         << '\n';

    cout << "NVDA : "
         << exchange.getMatchingEngine(Symbol::NVDA).trades.size()
         << '\n';

    cout << "TSLA : "
         << exchange.getMatchingEngine(Symbol::TSLA).trades.size()
         << '\n';

    cout << "\nHardware Threads : "
         << thread::hardware_concurrency()
         << '\n';

    return 0;
}