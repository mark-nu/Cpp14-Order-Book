#pragma once

#include <ostream>
#include "OrderBook.h"

class Reporter
{
private:
    std::shared_ptr<OrderBook> _orderBook;

public:
    Reporter() : _orderBook(new OrderBook) {}
    ~Reporter() = default;

    void printCurrentOrderBook(std::ostream &os) const;
    void printMidQuote(std::ostream &os) const;
};