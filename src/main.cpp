#include <iostream>
#include "../include/matchingengine.hpp"

using namespace std;

int main()
{
    MatchingEngine engine;
    engine.processOrder(
        Order(1, 1, Side::BUY, OrderType::LIMIT, 100, 500, 1)
    );

    engine.processOrder(
        Order(2, 1, Side::BUY, OrderType::LIMIT, 80, 495, 2)
    );

    engine.processOrder(
        Order(3, 1, Side::BUY, OrderType::LIMIT, 60, 490, 3)
    );

    engine.processOrder(
        Order(4, 2, Side::SELL, OrderType::LIMIT, 50, 485, 4)
    );

    engine.processOrder(
        Order(5, 2, Side::SELL, OrderType::LIMIT, 70, 500, 5)
    );

    engine.processOrder(
        Order(6, 2, Side::SELL, OrderType::LIMIT, 120, 505, 6)
    );
    cout << "Trades\n";
    for(const auto& trade : engine.trades){
        cout << trade.tradeId << " "
             << trade.buyOrderId << " "
             << trade.sellOrderId << " "
             << trade.quantity << " "
             << trade.price << '\n';
    }
    cout << "\nRemaining Buys\n";
    for(const auto& order : engine.orderBook.buys){
        cout << order.orderId << " "
             << order.quantity << " "
             << order.price << '\n';
    }
    cout << "\nRemaining Sells\n";
    for(const auto& order : engine.orderBook.sells){
        cout << order.orderId << " "
             << order.quantity << " "
             << order.price << '\n';
    }
    return 0;
}