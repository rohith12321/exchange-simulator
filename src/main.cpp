#include <iostream>
#include "../include/exchange.hpp"

using namespace std;

int main(){
    Exchange exchange;

    exchange.addTrader(1);
    exchange.addTrader(2);

    exchange.submitOrder(
        Order(1, 1, Side::BUY, OrderType::LIMIT, 100, 500, 1)
    );

    exchange.submitOrder(
        Order(2, 1, Side::BUY, OrderType::LIMIT, 80, 495, 2)
    );

    exchange.submitOrder(
        Order(3, 1, Side::BUY, OrderType::LIMIT, 60, 490, 3)
    );

    exchange.submitOrder(
        Order(4, 2, Side::SELL, OrderType::LIMIT, 50, 485, 4)
    );

    exchange.submitOrder(
        Order(5, 2, Side::SELL, OrderType::LIMIT, 70, 500, 5)
    );

    exchange.submitOrder(
        Order(6, 2, Side::SELL, OrderType::LIMIT, 120, 505, 6)
    );

    auto& engine = exchange.getMatchingEngine();

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

    cout << "\nTrader 1\n";
    cout << exchange.getTrader(1).getCash() << " "
         << exchange.getTrader(1).getPosition() << '\n';

    cout << "\nTrader 2\n";
    cout << exchange.getTrader(2).getCash() << " "
         << exchange.getTrader(2).getPosition() << '\n';

    return 0;
}