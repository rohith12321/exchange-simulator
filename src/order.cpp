#include "../include/order.hpp"
using namespace std;

Order::Order(
    long long orderId, 
    int traderId, 
    Side side, 
    OrderType type, 
    int quantity, 
    double price, 
    long long timestamp
)
{
    this -> orderId = orderId;
    this -> traderId = traderId;
    this -> side = side;
    this -> type = type;
    this -> quantity = quantity;
    this -> price = price;
    this -> timestamp = timestamp;
}