#pragma once

#include "utils/Common.h"
using namespace common;

class Order
{
public:
    enum class Action : char
    {
        ADD = 'A',
        CANCEL = 'X',
        MODIFY = 'M',
        TRADE = 'T',
    };

    enum class Side : char
    {
        BUY = 'B',
        SELL = 'S',
    };

public:
    Order() = default;
    ~Order() = default;
    Order(const Order &) = delete;
    Order &operator=(const Order &) = delete;

    auto getAction() { return _action; }
    auto getOrderId() { return _orderId; }
    auto getSide() { return _side; }
    auto getPrice() { return _price; }
    auto getQty() { return _qty; }

private:
    char _action = 0;
    OrderId _orderId = 0;
    char _side = 0;
    Price _price = 0.0;
    Quantity _qty = 0;
};