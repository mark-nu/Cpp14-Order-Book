#include "Order.h"
#include <sstream>
#include <iostream>

void Order::parse(const char *line, size_t len)
{
    _orderId = 0;
    _qty = 0;
    _price = 0;
    auto i = 0;
    bool isTrade = false;

    auto validLine = [&]() -> bool
    {
        for (; i < len; ++i)
        {
            if (line[i] == ' ')
            {
                continue;
            }

            if (line[i] == '/' && line[i + 1] == '/')
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        return false;
    };

    auto findNextField = [&]() -> void
    {
        for (; i < len; ++i)
        {
            if (line[i] == ',')
            {
                ++i;
                return;
            }
            else if (line[i] == ' ')
            {
                continue;
            }
        }
    };

    auto parseAction = [&]() -> bool
    {
        for (; i < len; ++i)
        {
            switch (line[i])
            {
            case 'A':
                _action = Action::ADD;
                break;
            case 'X':
                _action = Action::CANCEL;
                break;
            case 'M':
                _action = Action::MODIFY;
                break;
            case 'T':
                _action = Action::TRADE;
                isTrade = true;
                break;
            default: /* error handling */
                break;
            }

            if (_action != Action::ADD && _action != Action::CANCEL && _action != Action::MODIFY && _action != Action::TRADE)
            {
                return false;
            }
            else
            {
                ++i;
                return true;
            }
        }

        return false;
    };

    auto parseOrderId = [&]() -> void
    {
        if (_action == Action::TRADE)
        {
            return;
        }

        for (; i < len; ++i)
        {
            if (std::isdigit(line[i]))
            {
                _orderId *= 10;
                _orderId += line[i] - '0';
            }
            else
            {
                ++i;
                return;
            }
        }
    };

    auto parseSide = [&]() -> bool
    {
        for (; i < len; ++i)
        {
            switch (line[i])
            {
            case 'B':
                _side = Side::BUY;
                break;
            case 'S':
                _side = Side::SELL;
                break;
            default: /* error handling */
                break;
            }

            if (_side != Side::BUY && _side != Side::SELL)
            {
                return false;
            }
            else
            {
                ++i;
                return true;
            }
        }

        return false;
    };

    auto parseQuantity = [&]() -> void
    {
        for (; i < len; ++i)
        {
            if (std::isdigit(line[i]))
            {
                _qty *= 10;
                _qty += line[i] - '0';
            }
            else
            {
                ++i;
                return;
            }
        }
    };

    auto parsePrice = [&]() -> void
    {
        for (; i < len; ++i)
        {
            if (std::isdigit(line[i]))
            {
                _price *= 10;
                _price += line[i] - '0';
            }
            else
            {
                ++i;
                return;
            }
        }
    };

    if (!validLine())
    {
        return;
    }
    parseAction();
    findNextField();

    if (isTrade)
    {
        parseQuantity();
        findNextField();
        parsePrice();
    }
    else
    {
        parseOrderId();
        findNextField();
        parseSide();
        findNextField();
        parseQuantity();
        findNextField();
        parsePrice();
    }

    return;
}