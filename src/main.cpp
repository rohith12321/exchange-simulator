#include <iostream>
#include "../include/order.hpp"
using namespace std;

int main(){
    Order o(1, 42, Side::BUY, OrderType::LIMIT, 100, 500.0, 1);
    cout << o.orderId << '\n';
    cout << o.traderId << '\n';
    cout << o.quantity << '\n';
    cout << o.price << '\n';
    return 0;
}
