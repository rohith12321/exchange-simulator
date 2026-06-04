#include <iostream>
#include "../include/order.hpp"
#include "../include/orderbook.hpp"
using namespace std;

int main(){
    Order o(1, 42, Side::BUY, OrderType::LIMIT, 100, 500.27, 1);
    cout << o.orderId << '\n';
    cout << o.traderId << '\n';
    cout << o.quantity << '\n';
    cout << o.price << '\n';
    OrderBook book;
    book.addOrder(o);
    Order o1(1, 1, Side::BUY, OrderType::LIMIT, 100, 500, 10);
    Order o2(2, 1, Side::BUY, OrderType::LIMIT, 100, 500, 20);
    Order o3(3, 1, Side::BUY, OrderType::LIMIT, 100, 500, 25);
    book.addOrder(o1);
    book.addOrder(o2);
    book.addOrder(o3);
    for(const auto& order : book.buys){
        cout << order.price << " " << order.timestamp << "\n";
    }
    return 0;
}
