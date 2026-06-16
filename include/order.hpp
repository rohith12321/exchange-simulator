#pragma once

enum class Side{
    BUY,
    SELL
};

enum class OrderType{
    MARKET,
    LIMIT
};

enum class OrderStatus{
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};

class Order{
public:
    long long orderId;
    int traderId;
    Side side;
    OrderType type;
    OrderStatus status;
    int quantity;
    long long price;
    long long timestamp;

    Order(
        long long orderId,
        int traderId,
        Side side,
        OrderType type,
        int quantity,
        long long price,
        long long timestamp
    );
};