#include "FeedHandler.h"
#include "Order.h"
#include <iostream>

void FeedHandler::processMessage(const char *line, size_t lineLength)
{
    Order o;

    o.parse(line, lineLength);

    switch (o.getAction())
    {
    case static_cast<char>(Order::Action::ADD):
        _orderBook->AddOrder(o.getOrderId(), o);
        break;
    case static_cast<char>(Order::Action::CANCEL):
        std::cout << "CANCEL" << std::endl;
        break;
    case static_cast<char>(Order::Action::MODIFY):
        std::cout << "MODIFY" << std::endl;
        break;
    case static_cast<char>(Order::Action::TRADE):
        std::cout << "TRADE" << std::endl;
        break;
    }
}