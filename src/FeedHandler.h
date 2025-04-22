#pragma once

#include <string>
#include "OrderBook.h"
#include "Matcher.h"

class FeedHandler
{
private:
    std::shared_ptr<OrderBook> _orderBook;
    std::unique_ptr<Matcher> _matcher;

public:
    explicit FeedHandler(std::shared_ptr<OrderBook> book)
        : _orderBook(std::move(book)), _matcher(std::make_unique<Matcher>(_orderBook)) {}
    ~FeedHandler() = default;

    void processMessage(const char *line, size_t lineLength);
};