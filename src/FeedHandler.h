#include <string>

class FeedHandler
{
public:
    FeedHandler();

    void processMessage(const std::string &line);
};