#pragma once

#include <ostream>
#include "OrderBook.h"

class Reporter
{
private:
    std::shared_ptr<OrderBook> _orderBook;

public:
    explicit Reporter(std::shared_ptr<OrderBook> book)
        : _orderBook(std::move(book)) {}
    ~Reporter() = default;

    void printCurrentOrderBook(std::ostream &os) const;
    void printMidQuote(std::ostream &os) const;
};