#include "FeedHandler.h"
#include "../utils/Order.h"
#include <iostream>

void FeedHandler::processMessage(const char *line, size_t lineLength)
{
    auto order = std::make_shared<Order>();
    order->parse(line, lineLength);

    switch (order->getAction())
    {
    case static_cast<char>(Order::Action::ADD):
        _orderBook->addOrder(order);
        break;
    case static_cast<char>(Order::Action::CANCEL):
        _orderBook->cancelOrder(order->getOrderId());
        break;
    case static_cast<char>(Order::Action::MODIFY):
        _orderBook->modifyOrder(order);
        break;
    case static_cast<char>(Order::Action::TRADE):
        _orderBook->trade(order->getQty(), order->getPrice());
        break;
    }
}