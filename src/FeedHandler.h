#pragma once

#include <string>
#include "OrderBook.h"

class FeedHandler
{
private:
    std::shared_ptr<OrderBook> _orderBook;

public:
    FeedHandler() : _orderBook(new OrderBook) {}
    ~FeedHandler() = default;

    void processMessage(const char *line, size_t lineLength);
};