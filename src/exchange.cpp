#include "../include/exchange.hpp"
#include <stdexcept>
#include <atomic>
#include <chrono>

using Clock = std::chrono::steady_clock;

std::atomic<long long> validateTime{0};
std::atomic<long long> ordersLockTime{0};
std::atomic<long long> completionLockTime{0};
std::atomic<long long> queuePushTime{0};

using namespace std;

Exchange::Exchange(){
    for(int i = 0; i < 5; i++){
        Symbol symbol = static_cast<Symbol>(i);

        symbolEngines[i].matchingThread = thread([this, symbol](){

            auto& engine = symbolEngines[toIndex(symbol)];

            Order order;

            while(engine.orderQueue.pop(order)){
                if(order.type == OrderType::MARKET &&
                   !validateMarketOrder(order)){
                    {
                        lock_guard<mutex> lock(ordersMutex);

                        Order& storedOrder =
                            orders.at(order.orderId);

                        storedOrder.quantity = 0;
                        storedOrder.status =
                            OrderStatus::CANCELLED;
                    }

                    {
                        lock_guard<mutex> lock(completionMutex);
                        pendingOrders--;
                    }

                    completionCv.notify_all();
                    continue;
                }

                auto newTrades =
                    engine.matchingEngine.processOrder(order);

                for(const auto& trade : newTrades){
                    settleTrade(trade);
                }

                if(order.type == OrderType::MARKET){
                    lock_guard<mutex> lock(ordersMutex);

                    Order& storedOrder =
                        orders.at(order.orderId);

                    storedOrder.quantity = 0;

                    if(!newTrades.empty()){
                        storedOrder.status =
                            OrderStatus::FILLED;
                    }
                    else{
                        storedOrder.status =
                            OrderStatus::CANCELLED;
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
}

Exchange::~Exchange(){
    for(auto& engine : symbolEngines){
        engine.orderQueue.stop();
    }

    for(auto& engine : symbolEngines){
        if(engine.matchingThread.joinable()){
            engine.matchingThread.join();
        }
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

    auto& engine = symbolEngines[toIndex(order.symbol)];

    const auto& sells =
        engine.matchingEngine.orderBook.sells;

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

    auto t0 = Clock::now();

    if(traders.find(order.traderId) == traders.end()){
        throw runtime_error("Unknown Trader");
    }

    if(!validateOrder(order)){
        throw runtime_error("Risk check failed");
    }

    auto t1 = Clock::now();

    {
        lock_guard<mutex> lock(ordersMutex);

        if(orders.count(order.orderId)){
            throw runtime_error("Duplicate Order ID");
        }

        orders.emplace(order.orderId, order);
    }

    auto t2 = Clock::now();

    {
        lock_guard<mutex> lock(completionMutex);
        pendingOrders++;
    }

    auto t3 = Clock::now();

    auto& engine = symbolEngines[toIndex(order.symbol)];
    engine.orderQueue.push(order);

    auto t4 = Clock::now();

    validateTime +=
        chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();

    ordersLockTime +=
        chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

    completionLockTime +=
        chrono::duration_cast<chrono::nanoseconds>(t3 - t2).count();

    queuePushTime +=
        chrono::duration_cast<chrono::nanoseconds>(t4 - t3).count();
}

Trader& Exchange::getTrader(int traderId){
    return traders.at(traderId);
}

Order& Exchange::getOrder(int orderId){
    return orders.at(orderId);
}

MatchingEngine& Exchange::getMatchingEngine(Symbol symbol){
    return symbolEngines[toIndex(symbol)].matchingEngine;
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

void Exchange::waitUntilIdle(){
    unique_lock<mutex> lock(completionMutex);

    completionCv.wait(lock, [this](){
        return pendingOrders == 0;
    });
}