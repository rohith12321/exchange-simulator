#include <iostream>

#include "../include/exchange.hpp"

using namespace std;

int main(){
    Exchange exchange;

    exchange.addTrader(1, 1000000000);
    exchange.addTrader(2, 1000000000);

    int id = 1;
    int ts = 1;

    // ---------------- AAPL ----------------

    exchange.submitOrder(
        Order(id++, 1, Symbol::AAPL,
              Side::BUY, OrderType::LIMIT,
              10, 100, ts++)
    );

    exchange.submitOrder(
        Order(id++, 2, Symbol::AAPL,
              Side::SELL, OrderType::LIMIT,
              4, 100, ts++)
    );

    exchange.submitOrder(
        Order(id++, 2, Symbol::AAPL,
              Side::SELL, OrderType::LIMIT,
              3, 99, ts++)
    );

    // ---------------- GOOG ----------------

    exchange.submitOrder(
        Order(id++, 2, Symbol::GOOG,
              Side::SELL, OrderType::LIMIT,
              5, 200, ts++)
    );

    exchange.submitOrder(
        Order(id++, 1, Symbol::GOOG,
              Side::BUY, OrderType::LIMIT,
              2, 200, ts++)
    );

    exchange.submitOrder(
        Order(id++, 1, Symbol::GOOG,
              Side::BUY, OrderType::LIMIT,
              5, 200, ts++)
    );

    // ---------------- MSFT ----------------

    exchange.submitOrder(
        Order(id++, 1, Symbol::MSFT,
              Side::BUY, OrderType::LIMIT,
              8, 300, ts++)
    );

    exchange.submitOrder(
        Order(id++, 2, Symbol::MSFT,
              Side::SELL, OrderType::LIMIT,
              10, 300, ts++)
    );

    // ---------------- NVDA ----------------

    exchange.submitOrder(
        Order(id++, 2, Symbol::NVDA,
              Side::SELL, OrderType::LIMIT,
              6, 400, ts++)
    );

    // unmatched buy

    exchange.submitOrder(
        Order(id++, 1, Symbol::NVDA,
              Side::BUY, OrderType::LIMIT,
              3, 390, ts++)
    );

    // ---------------- TSLA ----------------

    exchange.submitOrder(
        Order(id++, 1, Symbol::TSLA,
              Side::BUY, OrderType::LIMIT,
              5, 500, ts++)
    );

    exchange.submitOrder(
        Order(id++, 2, Symbol::TSLA,
              Side::SELL, OrderType::LIMIT,
              5, 500, ts++)
    );

    exchange.waitUntilIdle();

    auto printBook = [&](Symbol symbol, const string& name){

        auto& engine = exchange.getMatchingEngine(symbol);

        cout << "========== "
             << name
             << " ==========\n";

        cout << "Trades : "
             << engine.trades.size()
             << '\n';

        cout << "Buy Orders : "
             << engine.orderBook.buys.size()
             << '\n';

        cout << "Sell Orders : "
             << engine.orderBook.sells.size()
             << "\n\n";
    };

    printBook(Symbol::AAPL, "AAPL");
    printBook(Symbol::GOOG, "GOOG");
    printBook(Symbol::MSFT, "MSFT");
    printBook(Symbol::NVDA, "NVDA");
    printBook(Symbol::TSLA, "TSLA");

    cout << "========== Traders ==========\n";

    cout << "Trader1 Cash : "
         << exchange.getTrader(1).getCash()
         << '\n';

    cout << "Trader1 Position : "
         << exchange.getTrader(1).getPosition()
         << '\n';

    cout << "Trader2 Cash : "
         << exchange.getTrader(2).getCash()
         << '\n';

    cout << "Trader2 Position : "
         << exchange.getTrader(2).getPosition()
         << '\n';

    return 0;
}