#include <iostream>

#include "../include/exchange.hpp"

using namespace std;

int main(){
    Exchange exchange;

    exchange.addTrader(1, 1000000000);
    exchange.addTrader(2, 1000000000);

    const int N = 10000;

    for(int i = 0; i < N; i++){
        exchange.submitOrder(
            Order(i + 1, 1, Side::BUY, OrderType::LIMIT, 1, 500, i + 1)
        );
    }

    for(int i = 0; i < N; i++){
        exchange.submitOrder(
            Order(N + i + 1, 2, Side::SELL, OrderType::LIMIT, 1, 500, N + i + 1)
        );
    }

    auto& engine = exchange.getMatchingEngine();

    cout << "Trades: " << engine.trades.size() << '\n';
    cout << "Buy Orders Remaining: " << engine.orderBook.buys.size() << '\n';
    cout << "Sell Orders Remaining: " << engine.orderBook.sells.size() << '\n';

    cout << "Trader 1 Cash: " << exchange.getTrader(1).getCash() << '\n';
    cout << "Trader 1 Position: " << exchange.getTrader(1).getPosition() << '\n';

    cout << "Trader 2 Cash: " << exchange.getTrader(2).getCash() << '\n';
    cout << "Trader 2 Position: " << exchange.getTrader(2).getPosition() << '\n';

    return 0;
}