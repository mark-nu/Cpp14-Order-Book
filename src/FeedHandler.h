#include <string>

class FeedHandler
{
public:
    FeedHandler() = default;

    void processMessage(const std::string &line);
};