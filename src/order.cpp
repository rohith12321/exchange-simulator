#include "../include/order.hpp"
using namespace std;

Order::Order(
    long long orderId,
    int traderId,
    Symbol symbol,
    Side side,
    OrderType type,
    int quantity,
    long long price,
    long long timestamp
)
    : orderId(orderId),
      traderId(traderId),
      symbol(symbol),
      side(side),
      type(type),
      status(OrderStatus::NEW),
      quantity(quantity),
      price(price),
      timestamp(timestamp)
{
}