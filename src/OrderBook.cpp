#include "OrderBook.h"
#include <iostream>

std::shared_ptr<Order> OrderBook::GetOrder(const OrderId &orderId)
{
    if (_orderMap.find(orderId) != _orderMap.end())
    {
        return _orderMap[orderId];
    }

    std::cout << "Did not find order" << std::endl;
    return nullptr;
}

void OrderBook::AddOrder(std::shared_ptr<Order> order)
{
    auto it = _orderMap.emplace(order->getOrderId(), order);
    if (!it.second)
    {
        std::cout << "Duplicate" << std::endl;
        return;
    }

    _orderQueue.push(order);
    if (order->getSide() == static_cast<char>(Order::Side::BUY))
    {
        _buyOrders[order->getPrice()].push(order);
    }
    else
    {
        _sellOrders[order->getPrice()].push(order);
    }
}

void OrderBook::ModifyOrder(const OrderId &orderId)
{
}

void OrderBook::CancelOrder(const OrderId &orderId)
{
    _orderMap.erase(orderId);
}
