#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <memory>

#include "../../utils/Order.h"
#include "../../src/OrderBook.h"
#include "../../src/FeedHandler.h"
#include "../../src/Reporter.h"

using namespace common;

TEST(OrderBookIntegrationTest, EndToEndRobust)
{
    auto bookPtr = std::make_shared<OrderBook>();
    FeedHandler feed(bookPtr);
    Reporter reporter(bookPtr);

    std::ifstream infile;
    infile.open("../tests/test1.txt");
    ASSERT_TRUE(infile.is_open()) << "Cannot open ../tests/test1.txt";

    std::string line;
    std::vector<std::string> midquotes;
    size_t processed = 0;

    while (std::getline(infile, line))
    {
        std::string t = line;
        t.erase(0, t.find_first_not_of(" 	"));
        if (t.empty() || t.rfind("//", 0) == 0)
            continue;

        feed.processMessage(line.c_str(), line.size());
        ++processed;

        std::ostringstream mos;
        reporter.printMidQuote(mos);
        midquotes.push_back(mos.str());
    }

    // Ensure we got a midquote for every valid line
    EXPECT_EQ(midquotes.size(), processed);
    EXPECT_EQ(processed, 26u);

    // helper: extract the number after the colon, or return NaN if "NAN"
    auto parseMid = [&](const std::string &s) -> double
    {
        auto p = s.find(':');
        if (p == std::string::npos)
        {
            throw std::runtime_error("bad midquote: " + s);
        }
        std::string num = s.substr(p + 1);
        if (num.find("NAN") != std::string::npos)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return std::stod(num);
    };

    // Spot-check some midquotes
    EXPECT_TRUE(std::isnan(parseMid(midquotes[0])));
    EXPECT_NEAR(parseMid(midquotes[1]), 1037.5, 1e-9);
    EXPECT_NEAR(parseMid(midquotes[3]), 1025.0, 1e-9);
    EXPECT_NEAR(parseMid(midquotes[5]), 1012.5, 1e-9);
    EXPECT_NEAR(parseMid(midquotes[9]), 1012.5, 1e-9);

    // Final book snapshot: check presence/absence of IDs
    std::ostringstream fos;
    reporter.printCurrentOrderBook(fos);
    const std::string fb = fos.str();
    std::vector<unsigned int> present = {100000, 100002, 100003, 100007, 100011};
    std::vector<unsigned int> absent = {100001, 100004, 100005, 100006, 100008, 100009, 100010, 100012};
    for (auto id : present)
    {
        EXPECT_NE(fb.find(std::to_string(id)), std::string::npos)
            << "Expected to find ID " << id;
    }
    for (auto id : absent)
    {
        EXPECT_EQ(fb.find(std::to_string(id)), std::string::npos)
            << "Did not expect ID " << id;
    }
}
