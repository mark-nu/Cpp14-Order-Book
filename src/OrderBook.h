#include <queue>
#include <map>
#include "OrderData.h"

using namespace common;

class OrderBook
{
public:
    std::queue<OrderData> orderQueue;

    std::map<Price, std::queue<OrderData>> sellOrdersByPrice;
    std::map<Price, std::queue<OrderData>, std::greater<Price>> buyOrdersByPrice;
};