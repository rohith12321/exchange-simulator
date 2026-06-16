#include <vector>
#include <iostream>

#include "../include/matchingengine.hpp"
using namespace std;

vector<Trade> MatchingEngine::processOrder(Order order){
    vector<Trade> newTrades;
    if(order.side == Side::BUY){
        auto it = orderBook.sells.begin();
        while(it != orderBook.sells.end() && it->price <= order.price && order.quantity > 0){
            int tradeQuantity = min(order.quantity, it->quantity);
            long long tradePrice = it->price;
            trades.emplace_back(
                nextTradeId++,
                order.orderId,
                it->orderId,
                tradeQuantity,
                tradePrice,
                order.timestamp
            );

            newTrades.push_back(trades.back());

            order.quantity -= tradeQuantity;

            Order updatedSellOrder = *it;
            updatedSellOrder.quantity -= tradeQuantity;
            orderBook.sells.erase(it);

            if(updatedSellOrder.quantity > 0)
                orderBook.addOrder(updatedSellOrder);

            it = orderBook.sells.begin();
        }

        if(order.quantity > 0)
            orderBook.addOrder(order);
    }
    else{
        auto it = orderBook.buys.begin();

        while(it != orderBook.buys.end() && it->price >= order.price && order.quantity > 0){
            int tradeQuantity = min(order.quantity, it->quantity);

            long long tradePrice = it->price;

            trades.emplace_back(
                nextTradeId++,
                it->orderId,
                order.orderId,
                tradeQuantity,
                tradePrice,
                order.timestamp
            );

            newTrades.push_back(trades.back());

            order.quantity -= tradeQuantity;

            Order updatedBuyOrder = *it;
            updatedBuyOrder.quantity -= tradeQuantity;
            orderBook.buys.erase(it);

            if(updatedBuyOrder.quantity > 0)
                orderBook.addOrder(updatedBuyOrder);

            it = orderBook.buys.begin();
        }

        if(order.quantity > 0)
            orderBook.addOrder(order);
    }

    return newTrades;
}