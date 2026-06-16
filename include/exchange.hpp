#pragma once
#include <unordered_map>
#include "matchingengine.hpp"
#include "trader.hpp"
#include "order.hpp"

class Exchange{
    private:
        MatchingEngine matchingEngine;
        std::unordered_map<int, Trader> traders;
        std::unordered_map<int, Order> orders;
    public:
        void addTrader(int TraderId, long long initialCash = 1000000);
        void submitOrder(Order order);
        void settleTrade(const Trade& trade);
        Trader& getTrader(int traderId);
        Order& getOrder(int orderId);
        MatchingEngine& getMatchingEngine();
};