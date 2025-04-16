#pragma once

#include <queue>
#include <map>
#include "Order.h"

class OrderBook
{
private:
    std::unordered_map<OrderId, Order> _orderMap;

    std::queue<Order> _orderQueue;

    std::map<Price, std::queue<Order>> _sellOrdersByPrice;
    std::map<Price, std::queue<Order>, std::greater<Price>> _buyOrdersByPrice;

public:
    OrderBook() = default;
    ~OrderBook() = default;

    Order &GetOrder(const OrderId &orderId);
    void AddOrder(const OrderId &orderId, Order &order);
    void ModifyOrder(const OrderId &orderId);
    void CancelOrder(const OrderId &orderId);
};