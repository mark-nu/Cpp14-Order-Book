#include <string>
#include <iostream>
#include <fstream>
#include "FeedHandler.h"
#include "OrderBook.h"
#include "Reporter.h"

int main(int argc, char **argv)
{
    auto book = std::make_shared<OrderBook>();
    FeedHandler feed(book);
    Reporter reporter(book);
    std::string line;
    const std::string filename(argv[1]);
    std::ifstream infile(filename.c_str(), std::ios::in);
    int counter = 0;
    while (std::getline(infile, line))
    {
        feed.processMessage(line.c_str(), line.size());
        if (++counter % 10 == 0)
        {
            reporter.printCurrentOrderBook(std::cerr);
        }
    }
    reporter.printCurrentOrderBook(std::cout);
    return 0;
}