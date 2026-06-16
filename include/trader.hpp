#pragma once

class Trader{
    private:
        int traderId;
        long long cash;
        long long position;

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