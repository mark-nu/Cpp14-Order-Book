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
    friend class Matcher;

private:
    OrderList _orderList;                 // FIFO of all orders
    SellOrdersByPrice _sellOrdersByPrice; // price -> list of sells
    BuyOrdersByPrice _buyOrdersByPrice;   // price -> list of buys

    ArrivalIterMap _arrivalIters; // orderId -> iterator in _orderList
    PriceListIterMap _priceIters; // orderId -> iterator in price list

    // trade accumulation
    Quantity _lastTradedQty{0};
    Price _lastTradePrice{0.0};

    template <typename MapType>
    void insertIntoBucket(MapType &book, const OrderPtr &order)
    {
        auto it = book.find(order->getPrice());
        if (it == book.end())
        {
            it = book.emplace(order->getPrice(), PriceBucket{}).first;
        }
        it->second.push_back(order);
        _priceIters[order->getOrderId()] = std::prev(it->second.end());
    }

    template <typename MapType>
    void removeFromBucket(MapType &book, const OrderPtr &order)
    {
        auto id = order->getOrderId();
        auto pitIt = _priceIters.find(id);
        if (pitIt == _priceIters.end())
            return;

        // grab the list‐node iterator
        auto listIt = pitIt->second;
        Price p = order->getPrice();

        // erase from the bucket
        auto mapIt = book.find(p);
        if (mapIt != book.end())
        {
            auto &bucket = mapIt->second;
            bucket.erase(listIt);
            if (bucket.empty()) // prune empty price‐level
            {
                book.erase(mapIt);
            }
        }

        _priceIters.erase(pitIt);
    }

public:
    OrderBook() = default;
    ~OrderBook() = default;
    OrderBook(const OrderBook &) = delete;
    OrderBook &operator=(const OrderBook &) = delete;

    OrderPtr getOrder(const OrderId &orderId);
    void addOrder(const OrderPtr &order);
    void modifyOrder(const OrderPtr &order);
    void cancelOrder(const OrderPtr &order);
    void trade(const Quantity &qty, const Price &price);
};