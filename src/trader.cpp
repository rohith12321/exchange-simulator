#include "../include/trader.hpp"

Trader::Trader(int traderId, long long initialCash)
    : traderId(traderId),
      cash(initialCash),
      position(0){
}

int Trader::getTraderId() const{
    return traderId;
}

long long Trader::getCash() const{
    return cash;
}

long long Trader::getPosition() const{
    return position;
}

void Trader::updateCash(long long amount){
    cash += amount;
}

void Trader::updatePosition(long long amount){
    position += amount;
}