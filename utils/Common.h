#include <tuple>;

namespace common
{
    using OrderId = unsigned int;
    using Quantity = unsigned int;
    using Price = double;

    using Order = std::tuple<Quantity, Price>;
}