#include <ostream>
class Reporter
{
public:
    Reporter() = default;
    ~Reporter() = default;

    void printCurrentOrderBook(std::ostream &os) const;
    void printMidQuote(std::ostream &os) const;
};