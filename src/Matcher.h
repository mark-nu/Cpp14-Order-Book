#pragma once

#include "OrderBook.h"

class Matcher
{
private:
    std::shared_ptr<OrderBook> _orderBook;

public:
    explicit Matcher(std::shared_ptr<OrderBook> book)
        : _orderBook(std::move(book)) {}
    ~Matcher() = default;
    Matcher(const Matcher &) = delete;
    Matcher &operator=(const Matcher &) = delete;

    void findMatch();
    void removeExecutedOrder(const OrderId &orderId);
};