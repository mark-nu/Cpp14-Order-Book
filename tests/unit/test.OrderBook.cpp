#include <gtest/gtest.h>
#include <sstream>
#include <memory>

#include "../../utils/Order.h"
#include "../../src/OrderBook.h"
#include "../../src/FeedHandler.h"
#include "../../src/Reporter.h"

using namespace common;

// Helper: capture midquote sequence
std::vector<std::string> captureMidQuotes(const std::vector<std::string> &lines)
{
    auto bookPtr = std::make_shared<OrderBook>();
    FeedHandler feed(bookPtr);
    Reporter reporter(bookPtr);
    std::vector<std::string> mids;
    for (const auto &line : lines)
    {
        bookPtr->getOrder(0); // ensure bookPtr is valid; no-op placeholder
        feed.processMessage(line.c_str(), line.size());
        std::ostringstream os;
        reporter.printMidQuote(os);
        mids.push_back(os.str());
    }
    return mids;
}

// Test Order::parse for ADD, CANCEL, MODIFY, TRADE formats
TEST(OrderParseTest, BasicFields)
{
    Order o;
    o.parse("A,12345,B,10,99");
    EXPECT_EQ(o.getAction(), Order::Action::ADD);
    EXPECT_EQ(o.getOrderId(), 12345u);
    EXPECT_EQ(o.getSide(), Order::Side::BUY);
    EXPECT_EQ(o.getQty(), 10u);
    EXPECT_DOUBLE_EQ(o.getPrice(), 99.0);

    o.parse("X,54321,S,5,88");
    EXPECT_EQ(o.getAction(), Order::Action::CANCEL);
    EXPECT_EQ(o.getOrderId(), 54321u);
    EXPECT_EQ(o.getSide(), Order::Side::SELL);
    EXPECT_EQ(o.getQty(), 5u);
    EXPECT_DOUBLE_EQ(o.getPrice(), 88.0);

    o.parse("M,10000,B,3,45.67");
    EXPECT_EQ(o.getAction(), Order::Action::MODIFY);
    EXPECT_EQ(o.getOrderId(), 10000u);
    EXPECT_EQ(o.getSide(), Order::Side::BUY);
    EXPECT_EQ(o.getQty(), 3u);
    EXPECT_DOUBLE_EQ(o.getPrice(), 45.67);

    o.parse("T,7,150.5");
    EXPECT_EQ(o.getAction(), Order::Action::TRADE);
    EXPECT_EQ(o.getQty(), 7u);
    EXPECT_DOUBLE_EQ(o.getPrice(), 150.5);
}

// Test midquote sequence against example
TEST(OrderBookMidQuoteTest, ExampleSequence)
{
    std::vector<std::string> msgs = {
        "A,1,B,0,0", // no buy or sell
        "A,100000,S,1,1075",
        "A,100001,B,9,1000",
        "A,100002,B,30,975",
        "A,100003,S,10,1050",
        "A,100004,B,10,950",
        "A,100005,S,2,1025",
    };
    auto mids = captureMidQuotes(msgs);
    EXPECT_NE(mids[0].find("NAN"), std::string::npos);
    EXPECT_NEAR(std::stod(mids[1]), 1037.5, 1e-9);
    EXPECT_NEAR(std::stod(mids[2]), 1037.5, 1e-9);
    EXPECT_NEAR(std::stod(mids[3]), 1025.0, 1e-9);
    EXPECT_NEAR(std::stod(mids[4]), 1025.0, 1e-9);
    EXPECT_NEAR(std::stod(mids[5]), 1012.5, 1e-9);
    EXPECT_NEAR(std::stod(mids[6]), 1012.5, 1e-9);
}

// Test simple add/modify/cancel in OrderBook internal state
TEST(OrderBookStateTest, AddModifyCancel)
{
    auto bookPtr = std::make_shared<OrderBook>();

    auto o1 = std::make_shared<Order>();
    o1->parse("A,10,B,5,100");
    bookPtr->addOrder(o1);

    auto o2 = std::make_shared<Order>();
    o2->parse("A,20,S,3,110");
    bookPtr->addOrder(o2);

    std::ostringstream os;
    Reporter reporter(bookPtr);
    reporter.printCurrentOrderBook(os);
    auto out = os.str();
    EXPECT_NE(out.find("Price: 110"), std::string::npos);
    EXPECT_NE(out.find("Price: 100"), std::string::npos);

    auto mod = std::make_shared<Order>();
    mod->parse("M,10,B,8,100");
    bookPtr->modifyOrder(mod);
    os.str("");
    os.clear();
    reporter.printCurrentOrderBook(os);
    out = os.str();
    EXPECT_NE(out.find("qty: 8"), std::string::npos);

    auto can = std::make_shared<Order>();
    can->parse("X,20,S,3,110");
    bookPtr->cancelOrder(can);
    os.str("");
    os.clear();
    reporter.printCurrentOrderBook(os);
    out = os.str();
    EXPECT_EQ(out.find("110"), std::string::npos);
}

// Test trades accumulate quantity
TEST(OrderBookTradeTest, CumulativeQuantity)
{
    auto bookPtr = std::make_shared<OrderBook>();
    Reporter rep(bookPtr);

    testing::internal::CaptureStdout();
    bookPtr->trade(2, 1025.0);
    bookPtr->trade(1, 1025.0);
    auto t1 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(t1, "3@1025\n");

    testing::internal::CaptureStdout();
    bookPtr->trade(1, 1000.0);
    auto t2 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(t2, "1@1000\n");

    testing::internal::CaptureStdout();
    bookPtr->trade(5, 1000.0);
    auto t3 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(t3, "6@1000\n");
}

// Test robustness: corrupted and duplicate IDs
TEST(OrderBookErrorTest, CorruptedAndDuplicate)
{
    auto bookPtr = std::make_shared<OrderBook>();

    auto bad = std::make_shared<Order>();
    bad->parse("Z,1,B,1,10");
    EXPECT_NO_THROW(bookPtr->addOrder(bad));

    auto o = std::make_shared<Order>();
    o->parse("A,1,B,1,10");
    bookPtr->addOrder(o);
    EXPECT_NO_THROW(bookPtr->addOrder(o));

    auto rem = std::make_shared<Order>();
    rem->parse("X,2,B,1,10");
    EXPECT_NO_THROW(bookPtr->cancelOrder(rem));

    // Trades don't touch bookPtr state, just printing
    EXPECT_NO_THROW(bookPtr->trade(1, 50.0));

    auto good = std::make_shared<Order>();
    good->parse("A,3,S,2,20");
    EXPECT_NO_THROW(bookPtr->addOrder(good));
}
