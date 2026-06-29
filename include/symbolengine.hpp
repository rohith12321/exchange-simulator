#pragma once

#include <thread>

#include "matchingengine.hpp"
#include "threadsafequeue.hpp"
#include "order.hpp"

struct SymbolEngine
{
    MatchingEngine matchingEngine;
    ThreadSafeQueue<Order> orderQueue;
    std::thread matchingThread;
};