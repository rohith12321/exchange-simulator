#include "../include/exchange.hpp"
#include <stdexcept>

using namespace std;

Exchange::Exchange(){
    matchingThread = thread([this](){
        Order order;

        while(orderQueue.pop(order)){
            if(order.type == OrderType::MARKET && !validateMarketOrder(order)){
                {
                    lock_guard<mutex> lock(ordersMutex);
                    Order& storedOrder = orders.at(order.orderId);
                    storedOrder.quantity = 0;
                    storedOrder.status = OrderStatus::CANCELLED;
                }

                {
                    lock_guard<mutex> lock(completionMutex);
                    pendingOrders--;
                }

                completionCv.notify_all();
                continue;
            }

            auto newTrades = matchingEngine.processOrder(order);

            for(const auto& trade : newTrades){
                settleTrade(trade);
            }

            if(order.type == OrderType::MARKET){
                lock_guard<mutex> lock(ordersMutex);

                Order& storedOrder = orders.at(order.orderId);
                storedOrder.quantity = 0;

                if(!newTrades.empty()){
                    storedOrder.status = OrderStatus::FILLED;
                }
                else{
                    storedOrder.status = OrderStatus::CANCELLED;
                }
            }

            {
                lock_guard<mutex> lock(completionMutex);
                pendingOrders--;
            }

            completionCv.notify_all();
        }
    });
}

Exchange::~Exchange(){
    orderQueue.stop();

    if(matchingThread.joinable()){
        matchingThread.join();
    }
}

void Exchange::addTrader(int traderId, long long initialCash){
    traders.emplace(
        traderId,
        Trader(traderId, initialCash)
    );
}

bool Exchange::validateOrder(const Order& order){
    const Trader& trader = traders.at(order.traderId);

    if(order.quantity <= 0){
        return false;
    }

    if(order.type == OrderType::LIMIT &&
       order.price <= 0){
        return false;
    }

    if(order.side == Side::BUY &&
       order.type == OrderType::LIMIT){

        long long requiredCash =
            1LL * order.quantity * order.price;

        if(trader.getCash() < requiredCash){
            return false;
        }
    }

    return true;
}

bool Exchange::validateMarketOrder(const Order& order){
    if(order.side != Side::BUY){
        return true;
    }

    const Trader& trader = traders.at(order.traderId);

    long long totalCost = 0;
    int remaining = order.quantity;

    const auto& sells =
        matchingEngine.orderBook.sells;

    for(const auto& sell : sells){
        if(remaining == 0){
            break;
        }

        int traded =
            min(remaining, sell.quantity);

        totalCost +=
            1LL * traded * sell.price;

        remaining -= traded;
    }

    if(remaining > 0){
        return false;
    }

    if(trader.getCash() < totalCost){
        return false;
    }

    return true;
}

void Exchange::submitOrder(Order order){
    if(traders.find(order.traderId) == traders.end()){
        throw runtime_error("Unknown Trader");
    }

    if(!validateOrder(order)){
        throw runtime_error("Risk check failed");
    }

    {
        lock_guard<mutex> lock(ordersMutex);

        if(orders.count(order.orderId)){
            throw runtime_error("Duplicate Order ID");
        }

        orders.emplace(order.orderId, order);
    }

    {
        lock_guard<mutex> lock(completionMutex);
        pendingOrders++;
    }

    orderQueue.push(order);
}

Trader& Exchange::getTrader(int traderId){
    return traders.at(traderId);
}

Order& Exchange::getOrder(int orderId){
    return orders.at(orderId);
}

MatchingEngine& Exchange::getMatchingEngine(){
    return matchingEngine;
}

void Exchange::settleTrade(const Trade& trade){
    lock_guard<mutex> lock(ordersMutex);

    Order& buyOrder = orders.at(trade.buyOrderId);
    Order& sellOrder = orders.at(trade.sellOrderId);

    Trader& buyer = traders.at(buyOrder.traderId);
    Trader& seller = traders.at(sellOrder.traderId);

    long long totalCost = 1LL * trade.quantity * trade.price;

    buyer.updateCash(-totalCost);
    buyer.updatePosition(trade.quantity);

    seller.updateCash(totalCost);
    seller.updatePosition(-trade.quantity);

    buyOrder.quantity -= trade.quantity;
    sellOrder.quantity -= trade.quantity;

    if(buyOrder.quantity == 0){
        buyOrder.status = OrderStatus::FILLED;
    }
    else{
        buyOrder.status = OrderStatus::PARTIALLY_FILLED;
    }

    if(sellOrder.quantity == 0){
        sellOrder.status = OrderStatus::FILLED;
    }
    else{
        sellOrder.status = OrderStatus::PARTIALLY_FILLED;
    }
}

Order& Exchange::getOrder(int orderId){
    return orders.at(orderId);
}

void Exchange::waitUntilIdle(){
    unique_lock<mutex> lock(completionMutex);

    completionCv.wait(lock, [this](){
        return pendingOrders == 0;
    });
}