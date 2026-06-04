#include "../include/orderbook.hpp"
using namespace std;

bool OrderBook::BuyCmp::operator()(const Order& a, const Order& b) const{
    if(a.price != b.price)
        return a.price > b.price;
    else if(a.timestamp != b.timestamp)
        return a.timestamp < b.timestamp;
    else
        return a.orderId < b.orderId;
}

bool OrderBook::SellCmp::operator()(const Order& a, const Order& b) const{
    if(a.price != b.price)
        return a.price < b.price;
    else if(a.timestamp != b.timestamp)
        return a.timestamp < b.timestamp;
    else
        return a.orderId < b.orderId;
}

void OrderBook::addOrder(const Order& order){
    if(order.side == Side::BUY)
        buys.insert(order);
    else
        sells.insert(order);
}

