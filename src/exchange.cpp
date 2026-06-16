#include "../include/exchange.hpp"

void Exchange::submitOrder(Order order){
    matchingEngine.processOrder(order);
}

MatchingEngine& Exchange::getMatchingEngine(){
    return matchingEngine;
}