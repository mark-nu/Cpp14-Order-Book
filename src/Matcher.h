#pragma once

#include "OrderBook.h"

class Matcher
{
private:
    std::unique_ptr<OrderBook> _orderBook;

public:
    Matcher() = default;
    ~Matcher() = default;

    void findMatch(const Price &price, const Quantity &quantity);
    void removeExecutedOrder(const OrderId &orderId);
};