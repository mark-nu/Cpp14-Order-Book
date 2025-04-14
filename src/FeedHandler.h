#include <string>

class FeedHandler
{
public:
    FeedHandler() = default;
    ~FeedHandler() = default;

    void processMessage(const std::string &line);
};