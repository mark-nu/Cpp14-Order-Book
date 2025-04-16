#include "OrderBook.h"
#include <iostream>

Order &OrderBook::GetOrder(const OrderId &orderId)
{
    if (_orderMap.find(orderId) != _orderMap.end())
    {
        return _orderMap[orderId];
    }

    std::cout << "Did not find order" << std::endl;
}

void OrderBook::AddOrder(const OrderId &orderId, Order &order)
{
    _orderMap.emplace(orderId, order);
    _orderQueue.push(order);
}

void OrderBook::ModifyOrder(const OrderId &orderId)
{
}

void OrderBook::CancelOrder(const OrderId &orderId)
{
}
