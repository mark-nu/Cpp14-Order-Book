#pragma once

#include <string>
#include "OrderBook.h"

class FeedHandler
{
private:
    std::shared_ptr<OrderBook> _orderBook;

public:
    explicit FeedHandler(std::shared_ptr<OrderBook> book)
        : _orderBook(std::move(book)) {}
    ~FeedHandler() = default;

    void processMessage(const char *line, size_t lineLength);
};