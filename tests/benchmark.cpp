#include <chrono>
#include <iomanip>
#include <iostream>
#include <atomic>
#include <thread>

#include "../include/exchange.hpp"

using namespace std;
using Clock = chrono::steady_clock;

constexpr int NUM_ORDERS = 1'000'000;
constexpr long long INITIAL_CASH = 1'000'000'000'000'000LL;

int main(){

    Exchange exchange;
    exchange.orders.reserve(NUM_ORDERS);
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

    auto totalStart = Clock::now();

    auto submitStart = Clock::now();

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

    auto submitEnd = Clock::now();

    cout << "Finished submitting orders.\n";

    auto processingStart = Clock::now();

    exchange.waitUntilIdle();

    auto processingEnd = Clock::now();

    auto submitNs =
        chrono::duration_cast<chrono::nanoseconds>(
            submitEnd - submitStart
        ).count();

    auto processingNs =
        chrono::duration_cast<chrono::nanoseconds>(
            processingEnd - processingStart
        ).count();

    auto totalNs =
        chrono::duration_cast<chrono::nanoseconds>(
            processingEnd - totalStart
        ).count();

    double submitSec = submitNs / 1e9;
    double processingSec = processingNs / 1e9;
    double totalSec = totalNs / 1e9;

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

    cout << "\nTiming Breakdown\n";
    cout << "----------------\n";

    cout << "Submission\n";
    cout << "  "
         << submitNs
         << " ns\n";
    cout << "  "
         << submitNs / 1000.0
         << " us\n";
    cout << "  "
         << submitNs / 1000000.0
         << " ms\n";
    cout << "  "
         << submitSec
         << " s\n\n";

    cout << "Processing (waitUntilIdle)\n";
    cout << "  "
         << processingNs
         << " ns\n";
    cout << "  "
         << processingNs / 1000.0
         << " us\n";
    cout << "  "
         << processingNs / 1000000.0
         << " ms\n";
    cout << "  "
         << processingSec
         << " s\n\n";

    cout << "Total\n";
    cout << "  "
         << totalNs
         << " ns\n";
    cout << "  "
         << totalNs / 1000.0
         << " us\n";
    cout << "  "
         << totalNs / 1000000.0
         << " ms\n";
    cout << "  "
         << totalSec
         << " s\n";

    cout << "\nThroughput\n";
    cout << "----------\n";

    cout << "Orders / Second : "
         << static_cast<long long>(
                NUM_ORDERS / totalSec
            )
         << '\n';

    cout << "Trades / Second : "
         << static_cast<long long>(
                totalTrades / totalSec
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

    extern std::atomic<long long> validateTime;
    extern std::atomic<long long> ordersLockTime;
    extern std::atomic<long long> completionLockTime;
    extern std::atomic<long long> queuePushTime;

    cout << "\n========== submitOrder() breakdown ==========\n";

    cout << "Validation          : "
        << validateTime.load() / 1e6
        << " ms\n";

    cout << "Orders Mutex        : "
        << ordersLockTime.load() / 1e6
        << " ms\n";

    cout << "Completion Mutex    : "
        << completionLockTime.load() / 1e6
        << " ms\n";

    cout << "Queue Push          : "
        << queuePushTime.load() / 1e6
        << " ms\n";

    return 0;
}