#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include "../include/exchange.hpp"

using namespace std;
using Clock = chrono::steady_clock;

constexpr int NUM_ORDERS = 1'000'000;
constexpr int NUM_TRADERS = 100;
constexpr long long INITIAL_CASH = 1'000'000'000'000'000LL;

int main(){

    Exchange exchange;
    exchange.orders.reserve(NUM_ORDERS);

    for(int i = 1; i <= NUM_TRADERS; i++){
        exchange.addTrader(i, INITIAL_CASH);
    }

    long long orderId = 1;
    long long timestamp = 1;

    cout << "=============================================\n";
    cout << " Concurrent Exchange Throughput Benchmark\n";
    cout << "=============================================\n\n";

    cout << "Submitting "
         << NUM_ORDERS
         << " orders...\n";

    mt19937 rng(42);

    uniform_int_distribution<int> traderDist(1, NUM_TRADERS);
    uniform_int_distribution<int> symbolDist(0, 4);
    uniform_int_distribution<int> sideDist(0, 1);
    uniform_int_distribution<int> quantityDist(1, 100);
    uniform_int_distribution<int> priceDist(100, 1000);

    auto totalStart = Clock::now();

    int accepted = 0;
    int rejected = 0;

    for(int i = 0; i < NUM_ORDERS; i++){

        int traderId = traderDist(rng);

        Symbol symbol =
            static_cast<Symbol>(symbolDist(rng));

        Side side =
            sideDist(rng)
                ? Side::BUY
                : Side::SELL;

        if(exchange.submitOrder(
            Order(
                orderId++,
                traderId,
                symbol,
                side,
                OrderType::LIMIT,
                quantityDist(rng),
                priceDist(rng),
                timestamp++
            )
        )){
            accepted++;
        }
        else{
            rejected++;
        }
    }

    cout << "Finished submitting orders.\n";

    exchange.waitUntilIdle();

    auto totalEnd = Clock::now();

    double totalSec =
        chrono::duration<double>(
            totalEnd - totalStart
        ).count();

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

    cout << "Orders Generated : "
         << NUM_ORDERS
         << '\n';

    cout << "Orders Accepted  : "
         << accepted
         << '\n';

    cout << "Orders Rejected  : "
         << rejected
         << '\n';

    cout << "Trades Executed  : "
         << totalTrades
         << '\n';

    cout << "\nThroughput\n";
    cout << "----------\n";

    cout << "Orders / Second : "
         << static_cast<long long>(
                accepted / totalSec
            )
         << '\n';

    cout << "Trades / Second : "
         << static_cast<long long>(
                totalTrades / totalSec
            )
         << '\n';

    cout << "\nHardware Threads : "
         << thread::hardware_concurrency()
         << '\n';

    return 0;
}