#include <iostream>
#include "../include/exchange.hpp"

using namespace std;

void printStatus(const Order& order){
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
}

int main(){
    Exchange exchange;

    exchange.addTrader(1);
    exchange.addTrader(2);
    exchange.addTrader(3);

    cout << "========================\n";
    cout << "LIMIT ORDER TESTS\n";
    cout << "========================\n";

    exchange.submitOrder(
        Order(1, 1, Side::BUY, OrderType::LIMIT, 100, 500, 1)
    );

    exchange.submitOrder(
        Order(2, 1, Side::BUY, OrderType::LIMIT, 80, 495, 2)
    );

    exchange.submitOrder(
        Order(3, 2, Side::SELL, OrderType::LIMIT, 50, 490, 3)
    );

    exchange.submitOrder(
        Order(4, 2, Side::SELL, OrderType::LIMIT, 70, 500, 4)
    );

    cout << "\n========================\n";
    cout << "MARKET ORDER TESTS\n";
    cout << "========================\n";

    exchange.submitOrder(
        Order(5, 3, Side::BUY, OrderType::MARKET, 120, 0, 5)
    );

    cout << "\n========================\n";
    cout << "CANCELLATION TESTS\n";
    cout << "========================\n";

    cout << "Cancel Order 2: "
         << exchange.cancelOrder(2) << '\n';

    cout << "Cancel Order 2 Again: "
         << exchange.cancelOrder(2) << '\n';

    cout << "Cancel Filled Order 1: "
         << exchange.cancelOrder(1) << '\n';

    cout << "\n========================\n";
    cout << "RISK CHECK TESTS\n";
    cout << "========================\n";

    exchange.addTrader(4, 1000);

    try{
        exchange.submitOrder(
            Order(6, 4, Side::BUY, OrderType::LIMIT, 100, 500, 6)
        );

        cout << "Risk Test Failed\n";
    }
    catch(const exception& e){
        cout << "Risk Test Passed: "
             << e.what() << '\n';
    }

    auto& engine = exchange.getMatchingEngine();

    cout << "\n========================\n";
    cout << "TRADE HISTORY\n";
    cout << "========================\n";

    for(const auto& trade : engine.trades){
        cout << trade.tradeId << " "
             << trade.buyOrderId << " "
             << trade.sellOrderId << " "
             << trade.quantity << " "
             << trade.price << '\n';
    }

    cout << "\n========================\n";
    cout << "ORDER BOOK\n";
    cout << "========================\n";

    cout << "\nBuys\n";
    for(const auto& order : engine.orderBook.buys){
        cout << order.orderId << " "
             << order.quantity << " "
             << order.price << '\n';
    }

    cout << "\nSells\n";
    for(const auto& order : engine.orderBook.sells){
        cout << order.orderId << " "
             << order.quantity << " "
             << order.price << '\n';
    }

    cout << "\n========================\n";
    cout << "ORDER STATES\n";
    cout << "========================\n";

    for(int orderId = 1; orderId <= 5; orderId++){
        Order& order = exchange.getOrder(orderId);

        cout << order.orderId << " "
             << order.quantity << " ";

        printStatus(order);

        cout << '\n';
    }

    cout << "\n========================\n";
    cout << "TRADERS\n";
    cout << "========================\n";

    for(int traderId = 1; traderId <= 3; traderId++){
        cout << "Trader " << traderId << '\n';
        cout << "Cash: "
             << exchange.getTrader(traderId).getCash()
             << '\n';

        cout << "Position: "
             << exchange.getTrader(traderId).getPosition()
             << "\n\n";
    }

    return 0;
}