#pragma once

#include <set>
#include "order.hpp"

class OrderBook
{
public:
    struct BuyCmp
    {
        bool operator()(const Order& a, const Order& b) const;
    };

    struct SellCmp
    {
        bool operator()(const Order& a, const Order& b) const;
    };

    std::set<Order, BuyCmp> buys;
    std::set<Order, SellCmp> sells;

    void addOrder(const Order& order);
};