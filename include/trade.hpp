#pragma once
class Trade{
    public:
        long long tradeId;
        long long buyOrderId;
        long long sellOrderId;
        int quantity;
        double price;
        long long timestamp;

        Trade(
            long long tradeId, 
            long long buyOrderId, 
            long long sellOrderId, 
            int quantity, 
            double price, 
            long long timestamp
        );
};