#include <iostream>
#include "../include/trade.hpp"
using namespace std;

Trade::Trade(
    long long tradeId, 
    long long buyOrderId, 
    long long sellOrderId, 
    int quantity, 
    long long price, 
    long long timestamp
)
{
    this -> tradeId = tradeId;
    this -> buyOrderId = buyOrderId;
    this -> sellOrderId = sellOrderId;
    this -> quantity = quantity;
    this -> price = price;
    this -> timestamp = timestamp;
}