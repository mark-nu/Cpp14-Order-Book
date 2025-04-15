#include "FeedHandler.h"
#include "Order.h"

void FeedHandler::processMessage(const std::string &line)
{
    Order o;

    o.parse(line, line.length());
}