#include "FeedHandler.h"
#include "../utils/Order.h"
#include <iostream>

void FeedHandler::processMessage(const char *line, size_t lineLength)
{
    auto order = std::make_shared<Order>();
    order->parse(line, lineLength);

    switch (order->getAction())
    {
    case Order::Action::ADD:
        _orderBook->addOrder(order);
        _matcher->findMatch();
        break;
    case Order::Action::CANCEL:
        _orderBook->cancelOrder(order);
        break;
    case Order::Action::MODIFY:
        _orderBook->modifyOrder(order);
        break;
    case Order::Action::TRADE:
        _orderBook->trade(order->getQty(), order->getPrice());
        break;
    }
}