#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "FeedHandler.h"
#include "OrderBook.h"
#include "Reporter.h"

int main(int argc, char **argv)
{
    using std::chrono::high_resolution_clock;
    high_resolution_clock::time_point start, end;
    using std::chrono::duration_cast;
    using std::chrono::nanoseconds;

    const int nFiles = argc - 1;
    const int nTrials = 3;

    std::vector<std::vector<double>> times(nFiles, std::vector<double>(nTrials, 0.0));

    for (int i = 0; i < nFiles; ++i)
    {
        const std::string filename(argv[i + 1]);

        for (int j = 0; j < nTrials; ++j)
        {
            std::ifstream infile(filename.c_str(), std::ios::in);

            if (!infile.is_open())
            {
                std::cerr << "Unable to open input file" << std::endl;
                return 1;
            }

            auto book = std::make_shared<OrderBook>();
            FeedHandler feed(book);
            Reporter reporter(book);
            std::string line;
            int counter = 0;
            start = high_resolution_clock::now();

            while (std::getline(infile, line))
            {
                feed.processMessage(line.c_str(), line.size());
                if (++counter % 10 == 0)
                {
                    reporter.printCurrentOrderBook(std::cerr);
                }

                reporter.printMidQuote(std::cerr);
            }
            reporter.printCurrentOrderBook(std::cout);

            end = high_resolution_clock::now();
            times[i][j] = duration_cast<nanoseconds>(end - start).count() * 1e-9;
        }
    }

    std::ofstream outfile("build/output.txt");
    if (!outfile.is_open())
    {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }

    outfile << std::setw(20) << "File";
    for (int tr = 1; tr <= nTrials; ++tr)
    {
        outfile << std::setw(20) << ("Run#" + std::to_string(tr));
    }
    outfile << "\n";

    // data rows
    outfile << std::setprecision(std::numeric_limits<double>::max_digits10);
    for (int fi = 0; fi < nFiles; ++fi)
    {
        outfile << std::setw(20) << argv[fi + 1];
        for (int tr = 0; tr < nTrials; ++tr)
        {
            outfile << std::setw(20) << (times[fi][tr] < 0 ? std::string("ERR") : std::to_string(times[fi][tr]));
        }

        double sum = 0;
        int cnt = 0;
        for (auto t : times[fi])
        {
            if (t >= 0)
            {
                sum += t;
                ++cnt;
            }
        }
        double avg = cnt ? sum / cnt : -1;
        if (avg < 0)
        {
            outfile << std::setw(20) << "ERR";
        }
        else
        {
            outfile << std::setw(20) << avg;
        }
        outfile << "\n";
    }

    outfile.close();

    return 0;
}