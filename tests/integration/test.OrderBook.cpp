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
    ASSERT_TRUE(infile.is_open()) << "Cannot open test1.txt";

    std::string line;
    std::vector<std::string> midquotes;
    std::vector<std::string> trades;

    while (std::getline(infile, line))
    {
        // Skip blank or comment lines
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        if (trimmed.empty() || trimmed.rfind("//", 0) == 0)
            continue;

        // Capture trade output if it's a TRADE message
        if (trimmed[0] == 'T')
        {
            testing::internal::CaptureStdout();
            feed.processMessage(line.c_str(), line.size());
            trades.push_back(testing::internal::GetCapturedStdout());
        }
        else
        {
            feed.processMessage(line.c_str(), line.size());
        }
        // Always record midquote
        std::ostringstream mos;
        reporter.printMidQuote(mos);
        midquotes.push_back(mos.str());
    }

    // Expect 26 processed messages
    EXPECT_EQ(midquotes.size(), 26u);
    EXPECT_EQ(trades.size(), 4u);

    // Verify trade outputs sequence
    std::vector<std::string> expTrades = {"2@1025\n", "3@1025\n", "20@1000\n", "25@1000\n"};
    for (size_t i = 0; i < expTrades.size(); ++i)
    {
        EXPECT_EQ(trades[i], expTrades[i]) << "Trade #" << i;
    }

    // Spot-check midquotes at key positions
    EXPECT_NE(midquotes[0].find("NAN"), std::string::npos);
    EXPECT_NEAR(std::stod(midquotes[1]), 1037.5, 1e-9);
    EXPECT_NEAR(std::stod(midquotes[3]), 1025.0, 1e-9);
    EXPECT_NEAR(std::stod(midquotes[5]), 1012.5, 1e-9);
    EXPECT_NEAR(std::stod(midquotes[9]), 1037.5, 1e-9); // after 10th msg

    // Final book snapshot: check presence/absence of IDs
    std::ostringstream fos;
    reporter.printCurrentOrderBook(fos);
    const std::string fb = fos.str();
    std::vector<unsigned int> present = {100000, 100001, 100002, 100003, 100006, 100007, 100011, 100012};
    std::vector<unsigned int> absent = {100004, 100005, 100008, 100009, 100010};
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
