#include "Matcher.h"

void Matcher::findMatch()
{
    auto &buys = _orderBook->_buyOrdersByPrice;
    auto &sells = _orderBook->_sellOrdersByPrice;

    while (!buys.empty() && !sells.empty())
    {
        auto buyPrice = buys.begin()->first;
        auto sellPrice = sells.begin()->first;

        if (buyPrice < sellPrice)
        {
            break;
        }

        auto &buyQueue = buys.begin()->second;
        auto &sellQueue = sells.begin()->second;

        auto buyOrder = buyQueue.front();
        auto sellOrder = sellQueue.front();

        Quantity traded = std::min(buyOrder->getQty(), sellOrder->getQty());

        _orderBook->trade(traded, sellPrice);

        buyOrder->setQty(buyOrder->getQty() - traded);
        sellOrder->setQty(sellOrder->getQty() - traded);

        if (buyOrder->getQty() == 0)
        {
            _orderBook->cancelOrder(buyOrder->getOrderId());
        }
        if (sellOrder->getQty() == 0)
        {
            _orderBook->cancelOrder(sellOrder->getOrderId());
        }
    }
}

void Matcher::removeExecutedOrder(const OrderId &orderId)
{
}
