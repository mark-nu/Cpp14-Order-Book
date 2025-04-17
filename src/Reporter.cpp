#include "Reporter.h"
#include <iostream>

void Reporter::printCurrentOrderBook(std::ostream &os) const
{
    OrderBook *orderBook = _orderBook.get();

    for (const auto &order : orderBook->_orderMap)
    {
        os << order.first << ": " << order.second->getQty() << std::endl;
    }
}

void Reporter::printMidQuote(std::ostream &os) const
{
}
