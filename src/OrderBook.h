#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include <map>
#include <iostream>
#include "../utils/Order.h"

using OrderPtr = std::shared_ptr<Order>;
using OrderList = std::list<OrderPtr>;
using PriceBucket = std::list<OrderPtr>;
using SellOrdersByPrice = std::map<Price, PriceBucket>;
using BuyOrdersByPrice = std::map<Price, PriceBucket, std::greater<Price>>;

// iterators into the two lists
using ArrivalIterMap = std::unordered_map<OrderId, OrderList::iterator>;
using PriceListIterMap = std::unordered_map<OrderId, PriceBucket::iterator>;

class OrderBook
{
    friend class Reporter;

private:
    OrderList _orderList;                 // FIFO of all orders
    SellOrdersByPrice _sellOrdersByPrice; // price -> list of sells
    BuyOrdersByPrice _buyOrdersByPrice;   // price -> list of buys

    ArrivalIterMap _arrivalIters; // orderId -> iterator in _orderList
    PriceListIterMap _priceIters; // orderId -> iterator in price list

    // trade accumulation
    Quantity _lastTradedQty{0};
    Price _lastTradePrice{0.0};

public:
    OrderBook() = default;
    ~OrderBook() = default;

    std::shared_ptr<Order> getOrder(const OrderId &orderId);
    void addOrder(std::shared_ptr<Order> order);
    void modifyOrder(std::shared_ptr<Order> order);
    void cancelOrder(const OrderId &orderId);
    void trade(const Quantity &qty, const Price &price);
};