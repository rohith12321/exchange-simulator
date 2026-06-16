#pragma once

#include <vector>
#include "order.hpp"
#include "trade.hpp"
#include "orderbook.hpp"

class MatchingEngine{
    public:
        OrderBook orderBook;
        std::vector<Trade> trades;
        inline static int nextTradeId = 1;
        std::vector<Trade> processOrder(Order order);
};
