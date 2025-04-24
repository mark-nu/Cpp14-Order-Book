#include "Order.h"
#include <cctype> // for std::isspace, std::isdigit

void Order::parse(const char *line, const size_t &len)
{
    const char *p = line;
    const char *end = line + len;

    // Skip leading whitespace
    while (p < end && std::isspace(*p))
    {
        ++p;
    }

    // Skip comment lines (“//…”)
    if (p + 1 < end && p[0] == '/' && p[1] == '/')
    {
        return;
    }

    // Parse action (A,X,M,T)
    _action = static_cast<Action>(*p++);

    // Skip one comma (or any whitespace+comma)
    auto skipComma = [&]()
    {
        while (p < end && (std::isspace(*p) || *p == ','))
            ++p;
    };
    skipComma();

    if (_action == Action::TRADE)
    {
        // Format: T,<qty>,<price>
        // qty
        while (p < end && std::isdigit(*p))
        {
            _qty = _qty * 10 + (*p - '0');
            ++p;
        }
        skipComma();

        // price
        while (p < end && std::isdigit(*p))
        {
            _price = _price * 10 + (*p - '0');
            ++p;
        }
    }
    else
    {
        // Format: A|X|M,<orderId>,<side>,<qty>,<price>
        // orderId
        while (p < end && std::isdigit(*p))
        {
            _orderId = _orderId * 10 + (*p - '0');
            ++p;
        }
        skipComma();

        // side
        _side = (*p == 'B' ? Side::BUY : Side::SELL);
        ++p;
        skipComma();

        // qty
        while (p < end && std::isdigit(*p))
        {
            _qty = _qty * 10 + (*p - '0');
            ++p;
        }
        skipComma();

        // price
        while (p < end && std::isdigit(*p))
        {
            _price = _price * 10 + (*p - '0');
            ++p;
        }
    }
}
