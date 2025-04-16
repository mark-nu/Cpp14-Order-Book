#include "Order.h"
#include <sstream>
#include <iostream>

void Order::parse(const char *line, size_t len)
{
    auto i = 0;

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
            _action = line[i];
            if (_action != static_cast<char>(Action::ADD) && _action != static_cast<char>(Action::CANCEL) && _action != static_cast<char>(Action::MODIFY) && _action != static_cast<char>(Action::TRADE))
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
        if (_action == static_cast<char>(Action::TRADE))
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
            _side = line[i];

            if (_side != static_cast<char>(Side::BUY) && _side != static_cast<char>(Side::SELL))
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

    validLine();
    parseAction();
    findNextField();
    parseOrderId();
    findNextField();
    parseSide();
    findNextField();
    parseQuantity();
    findNextField();
    parsePrice();

    return;
}