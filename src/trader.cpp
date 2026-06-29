#include "../include/trader.hpp"

using namespace std;

Trader::Trader(int traderId, long long initialCash)
    : traderId(traderId),
      cash(initialCash),
      position(0){
}

int Trader::getTraderId() const{
    return traderId;
}

long long Trader::getCash() const{

    lock_guard<mutex> lock(m);

    return cash;
}

long long Trader::getPosition() const{

    lock_guard<mutex> lock(m);

    return position;
}

void Trader::updateCash(long long amount){

    lock_guard<mutex> lock(m);

    cash += amount;
}

void Trader::updatePosition(long long amount){

    lock_guard<mutex> lock(m);

    position += amount;
}