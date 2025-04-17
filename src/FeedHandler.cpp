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
        std::cout << "ADD" << std::endl;
        _orderBook->addOrder(order);
        break;
    case static_cast<char>(Order::Action::CANCEL):
        _orderBook->cancelOrder(order->getOrderId());
        std::cout << "CANCEL" << std::endl;
        break;
    case static_cast<char>(Order::Action::MODIFY):
        _orderBook->modifyOrder(order->getOrderId());
        std::cout << "MODIFY" << std::endl;
        break;
    case static_cast<char>(Order::Action::TRADE):
        std::cout << "TRADE" << std::endl;
        break;
    }
}