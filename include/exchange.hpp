#pragma once

#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "threadsafequeue.hpp"
#include "matchingengine.hpp"
#include "trader.hpp"
#include "order.hpp"

class Exchange{
    private:
        MatchingEngine matchingEngine;
        std::unordered_map<int, Trader> traders;
        std::unordered_map<int, Order> orders;

        ThreadSafeQueue<Order> orderQueue;
        std::thread matchingThread;

        std::mutex ordersMutex;

        std::mutex completionMutex;
        std::condition_variable completionCv;
        int pendingOrders = 0;

    public:
        Exchange();
        ~Exchange();

        void addTrader(int traderId, long long initialCash = 1000000);
        void submitOrder(Order order);
        void settleTrade(const Trade& trade);

        Trader& getTrader(int traderId);
        Order& getOrder(int orderId);
        MatchingEngine& getMatchingEngine();

        bool cancelOrder(int orderId);
        bool validateOrder(const Order& order);
        bool validateMarketOrder(const Order& order);

        void waitUntilIdle();
};