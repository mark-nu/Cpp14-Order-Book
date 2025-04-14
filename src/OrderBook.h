#include <queue>
#include <map>
#include "Order.h"

class OrderBook
{
protected:
    std::unordered_map<OrderId, Order> _orderMap;

public:
    OrderBook() = default;
    ~OrderBook() = default;

    Order GetOrder(const OrderId &orderId);
    void AddOrder(const Order &order);
    void ModifyOrder(const OrderId &orderId);
    void CancelOrder(const OrderId &orderId);

    std::queue<Order> orderQueue;

    std::map<Price, std::queue<Order>> sellOrdersByPrice;
    std::map<Price, std::queue<Order>, std::greater<Price>> buyOrdersByPrice;
};