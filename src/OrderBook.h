#pragma once

#include <queue>
#include <map>
#include "../utils/Order.h"

using OrderMap = std::unordered_map<OrderId, std::shared_ptr<Order>>;
using OrderQueue = std::queue<std::shared_ptr<Order>>;
using SellOrdersByPrice = std::map<Price, std::queue<std::shared_ptr<Order>>>;
using BuyOrdersByPrice = std::map<Price, std::queue<std::shared_ptr<Order>>, std::greater<Price>>;

class OrderBook
{
    friend class Reporter;

private:
    OrderMap _orderMap;
    OrderQueue _orderQueue;

    SellOrdersByPrice _sellOrders;
    BuyOrdersByPrice _buyOrders;

public:
    OrderBook() = default;
    ~OrderBook() = default;

    std::shared_ptr<Order> GetOrder(const OrderId &orderId);
    void AddOrder(std::shared_ptr<Order> order);
    void ModifyOrder(const OrderId &orderId);
    void CancelOrder(const OrderId &orderId);
};