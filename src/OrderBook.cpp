#include "OrderBook.h"

std::shared_ptr<Order> OrderBook::getOrder(const OrderId &orderId)
{
    auto ait = _arrivalIters.find(orderId);
    if (ait == _arrivalIters.end())
    {
        std::cout << "Did not find order " << orderId << std::endl;
        return nullptr;
    }
    return *(ait->second);
}

void OrderBook::addOrder(std::shared_ptr<Order> order)
{
    OrderId id = order->getOrderId();
    if (_arrivalIters.count(id))
    {
        std::cout << "Duplicate order " << id << std::endl;
        return;
    }
    // 1) insert into global FIFO list
    auto lit = _orderList.insert(_orderList.end(), order);
    _arrivalIters[id] = lit;

    // 2) insert into corresponding price bucket
    auto &bucket = order->getSide() == static_cast<char>(Order::Side::BUY)
                       ? _buyOrdersByPrice[order->getPrice()]
                       : _sellOrdersByPrice[order->getPrice()];
    auto pit = bucket.insert(bucket.end(), order);
    _priceIters[id] = pit;
}

void OrderBook::modifyOrder(std::shared_ptr<Order> order)
{
    OrderId id = order->getOrderId();
    auto ait_it = _arrivalIters.find(id);
    if (ait_it == _arrivalIters.end())
    {
        std::cout << "Could not find order to modify: " << id << std::endl;
        return;
    }
    // existing pointer
    auto existing = *(ait_it->second);
    // update quantity
    existing->setQty(order->getQty());
    // price change?
    if (existing->getPrice() != order->getPrice())
    {
        Price old = existing->getPrice();
        // remove from old bucket
        auto pit_it = _priceIters[id];
        existing->getSide() == static_cast<char>(Order::Side::BUY)
            ? _buyOrdersByPrice[old].erase(pit_it)
            : _sellOrdersByPrice[old].erase(pit_it);
        // update price
        existing->setPrice(order->getPrice());
        // reinsert
        auto &newBucket = existing->getSide() == static_cast<char>(Order::Side::BUY)
                              ? _buyOrdersByPrice[existing->getPrice()]
                              : _sellOrdersByPrice[existing->getPrice()];
        auto newPit = newBucket.insert(newBucket.end(), existing);
        _priceIters[id] = newPit;
    }
}

void OrderBook::cancelOrder(const OrderId &orderId)
{
    // remove from FIFO
    if (_arrivalIters.count(orderId))
    {
        _orderList.erase(_arrivalIters[orderId]);
        _arrivalIters.erase(orderId);
    }
    // remove from price bucket
    if (_priceIters.count(orderId))
    {
        auto ptr = *(_priceIters[orderId]);
        Price p = ptr->getPrice();
        ptr->getSide() == static_cast<char>(Order::Side::BUY)
            ? _buyOrdersByPrice[p].erase(_priceIters[orderId])
            : _sellOrdersByPrice[p].erase(_priceIters[orderId]);
        _priceIters.erase(orderId);
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