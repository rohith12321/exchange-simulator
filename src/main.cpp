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

    cout << "\nOrders\n";

    exchange.cancelOrder(2);
    exchange.cancelOrder(6);

    for(int orderId = 1; orderId <= 6; orderId++){
        Order& order = exchange.getOrder(orderId);

        cout << order.orderId << " "
            << order.quantity << " ";

        if(order.status == OrderStatus::NEW){
            cout << "NEW";
        }
        else if(order.status == OrderStatus::PARTIALLY_FILLED){
            cout << "PARTIALLY_FILLED";
        }
        else if(order.status == OrderStatus::FILLED){
            cout << "FILLED";
        }
        else{
            cout << "CANCELLED";
        }

        cout << '\n';
    }
    cout << exchange.cancelOrder(1) << '\n';
    cout << exchange.cancelOrder(2) << '\n';
    return 0;
}