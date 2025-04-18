#include "Reporter.h"
#include <iostream>

void Reporter::printCurrentOrderBook(std::ostream &os) const
{
    OrderBook *orderBook = _orderBook.get();

    os << "----- SELL ORDERS -----" << std::endl;

    for (const auto &priceLevel : orderBook->_sellOrdersByPrice)
    {
        os << "Price: " << priceLevel.first << " | Qtys: ";
        for (const auto &order : priceLevel.second)
        {
            os << "[id: " << order->getOrderId() << ", qty: " << order->getQty() << "]";
        }
        os << std::endl;
    }

    os << "----- BUY ORDERS -----" << std::endl;

    for (const auto &priceLevel : orderBook->_buyOrdersByPrice)
    {
        os << "Price: " << priceLevel.first << " | Qtys: ";
        for (const auto &order : priceLevel.second)
        {
            os << "[id: " << order->getOrderId() << ", qty: " << order->getQty() << "]";
        }
        os << std::endl;
    }

    os << "-----------------------" << std::endl;
}

void Reporter::printMidQuote(std::ostream &os) const
{
    const auto &sells = _orderBook->_sellOrdersByPrice;
    const auto &buys = _orderBook->_buyOrdersByPrice;

    if (sells.empty() || buys.empty())
    {
        os << "Midquote: NAN" << std::endl;
        return;
    }

    double bestAsk = sells.begin()->first;
    double bestBid = buys.begin()->first;

    double mid = (bestAsk + bestBid) / 2.0;
    os << "Midquote: " << mid << std::endl;
}
