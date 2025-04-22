#include "OrderBook.h"

OrderPtr OrderBook::getOrder(const OrderId &orderId)
{
    auto ait = _arrivalIters.find(orderId);
    if (ait == _arrivalIters.end())
    {
        std::cout << "Did not find order " << orderId << std::endl;
        return nullptr;
    }
    return *(ait->second);
}

void OrderBook::addOrder(const OrderPtr &order)
{
    OrderId id = order->getOrderId();
    if (_arrivalIters.count(id))
    {
        std::cout << "Duplicate order " << id << std::endl;
        return;
    }

    // insert into global FIFO list
    _arrivalIters[id] = _orderList.insert(_orderList.end(), order);

    // insert into corresponding price bucket
    if (order->getSide() == Order::Side::BUY)
    {
        insertIntoBucket(_buyOrdersByPrice, order);
    }
    else
    {
        insertIntoBucket(_sellOrdersByPrice, order);
    }
}

void OrderBook::modifyOrder(const OrderPtr &incoming)
{
    OrderId id = incoming->getOrderId();
    auto existing = getOrder(id);
    if (!existing)
    {
        return;
    }

    // update the quantity in‐place
    existing->setQty(incoming->getQty());

    // if price has changed, move it between buckets
    if (existing->getPrice() != incoming->getPrice())
    {
        // 1) remove from old bucket
        if (existing->getSide() == Order::Side::BUY)
        {
            removeFromBucket(_buyOrdersByPrice, existing);
        }
        else
        {
            removeFromBucket(_sellOrdersByPrice, existing);
        }

        // 2) update price on the order
        existing->setPrice(incoming->getPrice());

        // 3) insert into the new bucket
        if (existing->getSide() == Order::Side::BUY)
        {
            insertIntoBucket(_buyOrdersByPrice, existing);
        }
        else
        {
            insertIntoBucket(_sellOrdersByPrice, existing);
        }
    }
}

void OrderBook::cancelOrder(const OrderPtr &order)
{
    auto id = order->getOrderId();

    auto existing = getOrder(id);
    if (!existing)
    {
        return;
    }

    auto ait = _arrivalIters.find(id);
    if (ait != _arrivalIters.end())
    {
        _orderList.erase(ait->second);
        _arrivalIters.erase(ait);
    }

    // 3) remove from the correct bucket using the stored order
    if (existing->getSide() == Order::Side::BUY)
    {
        removeFromBucket(_buyOrdersByPrice, existing);
    }
    else
    {
        removeFromBucket(_sellOrdersByPrice, existing);
    }
}

void OrderBook::trade(const Quantity &qty, const Price &price)
{
    if (price == _lastTradePrice)
    {
        _lastTradedQty += qty;
    }
    else
    {
        _lastTradePrice = price;
        _lastTradedQty = qty;
    }
    std::cout << _lastTradedQty << "@" << _lastTradePrice << std::endl;
}