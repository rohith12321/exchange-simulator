#pragma once

#include <mutex>

class Trader{
    private:
        int traderId;
        long long cash;
        long long position;

        mutable std::mutex m;

    public:
        Trader(
            int traderId,
            long long cash = 1000000
        );

        int getTraderId() const;
        long long getCash() const;
        long long getPosition() const;

        void updateCash(long long amount);
        void updatePosition(long long quantity);
};