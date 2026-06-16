#pragma once

#include "matchingengine.hpp"

class Exchange{
    private:
        MatchingEngine matchingEngine;
    public:
        void submitOrder(Order order);
        MatchingEngine& getMatchingEngine();
};