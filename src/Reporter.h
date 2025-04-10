#include <ostream>
class Reporter
{
public:
    Reporter();

    void printCurrentOrderBook(std::ostream &os) const;
    void printMidQuote(std::ostream &os) const;
};