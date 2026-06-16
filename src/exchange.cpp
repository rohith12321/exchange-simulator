#include "../include/exchange.hpp"
#include <stdexcept>

using namespace std;

void Exchange::addTrader(int traderId, long long initialCash){
    traders.emplace(
        traderId,
        Trader(traderId, initialCash)
    );
}

void Exchange::submitOrder(Order order){
    if(traders.find(order.traderId) == traders.end()){
        throw runtime_error("Unknown Trader");
    }

    if(orders.count(order.orderId)){
        throw runtime_error("Duplicate Order ID");
    }

    orders.emplace(order.orderId, order);

    auto newTrades = matchingEngine.processOrder(order);

    for(const auto& trade : newTrades){
        settleTrade(trade);
    }
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

bool Exchange::cancelOrder(int orderId){
    Order& order = orders.at(orderId);

    if(order.status == OrderStatus::FILLED)
        return false;

    if(order.status == OrderStatus::CANCELLED)
        return false;

    auto& book = matchingEngine.orderBook;

    if(order.side == Side::BUY)
        book.buys.erase(order);
    else
        book.sells.erase(order);

    order.status = OrderStatus::CANCELLED;

    return true;
}