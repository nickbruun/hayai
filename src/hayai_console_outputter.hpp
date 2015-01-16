#ifndef __HAYAI_CONSOLEOUTPUTTER
#define __HAYAI_CONSOLEOUTPUTTER
#include "hayai_outputter.hpp"
#include "hayai_console.hpp"

namespace hayai
{
    /// Console outputter.

    /// Prints the result to standard output.
    class ConsoleOutputter
        :   public Outputter
    {
    public:
        virtual void Begin(const std::size_t& benchmarksCount)
        {
            std::cout << std::fixed;
            std::cout << Console::TextGreen << "[==========]"
                      << Console::TextDefault << " Running "
                      << benchmarksCount
                      << (benchmarksCount == 1 ?
                          " benchmark." :
                          " benchmarks.")
                      << std::endl;
        }


        virtual void End(const std::size_t& benchmarksCount)
        {
            std::cout << Console::TextGreen << "[==========]"
                      << Console::TextDefault << " Ran " << benchmarksCount
                      << (benchmarksCount == 1 ?
                          " benchmark." :
                          " benchmarks.")
                      << std::endl;
        }


        virtual void BeginTest(const std::string& fixtureName,
                               const std::string& testName,
                               const std::string& parameters,
                               const std::size_t& runsCount,
                               const std::size_t& iterationsCount)
        {
            std::cout << Console::TextGreen << "[ RUN      ]"
                      << Console::TextYellow << " "
                      << fixtureName << "."
                      << testName
                      << parameters
                      << Console::TextDefault
                      << " (" << runsCount
                      << (runsCount == 1 ? " run, " : " runs, ")
                      << iterationsCount
                      << (iterationsCount == 1 ?
                          " iteration per run)" :
                          " iterations per run)")
                      << std::endl;
        }


        virtual void EndTest(const std::string& fixtureName,
                             const std::string& testName,
                             const std::string& parameters,
                             const TestResult& result)
        {
#define PAD(x) std::cout << std::setw(34) << x << std::endl;
#define PAD_DEVIATION(description,                                      \
                      deviated,                                         \
                      average,                                          \
                      unit)                                             \
            {                                                           \
                double _d_ =                                            \
                    double(deviated) - double(average);                 \
                                                                        \
                PAD(description <<                                      \
                    deviated << " " << unit << " (" <<                  \
                    (deviated < average ?                               \
                     Console::TextRed :                                 \
                     Console::TextGreen) <<                             \
                    (deviated > average ? "+" : "") <<                  \
                    _d_ << " " << unit << " / " <<                      \
                    (deviated > average ? "+" : "") <<                  \
                    (_d_ * 100.0 / average) << " %" <<                  \
                    Console::TextDefault << ")");                       \
            }
#define PAD_DEVIATION_INVERSE(description,                              \
                              deviated,                                 \
                              average,                                  \
                              unit)                                     \
            {                                                           \
                double _d_ =                                            \
                    double(deviated) - double(average);                 \
                                                                        \
                PAD(description <<                                      \
                    deviated << " " << unit << " (" <<                  \
                    (deviated > average ?                               \
                     Console::TextRed :                                 \
                     Console::TextGreen) <<                             \
                    (deviated > average ? "+" : "") <<                  \
                    _d_ << " " << unit << " / " <<                      \
                    (deviated > average ? "+" : "") <<                  \
                    (_d_ * 100.0 / average) << " %" <<                  \
                    Console::TextDefault << ")");                       \
            }

            std::cout << Console::TextGreen << "[     DONE ]"
                      << Console::TextYellow << " "
                      << fixtureName << "."
                      << testName
                      << parameters
                      << Console::TextDefault << " ("
                      << std::setprecision(6)
                      << (result.TimeTotal() / 1000000.0) << " ms)"
                      << std::endl;

            std::cout << Console::TextBlue << "[   RUNS   ] "
                      << Console::TextDefault
                      << "       Average time: "
                      << std::setprecision(3)
                      << result.RunTimeAverage() / 1000.0 << " us"
                      << std::endl;

            PAD_DEVIATION_INVERSE("Fastest: ",
                                  (result.RunTimeMinimum() / 1000.0),
                                  (result.RunTimeAverage() / 1000.0),
                                  "us");
            PAD_DEVIATION_INVERSE("Slowest: ",
                                  (result.RunTimeMaximum() / 1000.0),
                                  (result.RunTimeAverage() / 1000.0),
                                  "us");

            std::cout << std::setprecision(5);

            PAD("");
            PAD("Average performance: " <<
                result.RunsPerSecondAverage() << " runs/s");
            PAD_DEVIATION("Best performance: ",
                          result.RunsPerSecondMaximum(),
                          result.RunsPerSecondAverage(),
                          "runs/s");
            PAD_DEVIATION("Worst performance: ",
                          result.RunsPerSecondMinimum(),
                          result.RunsPerSecondAverage(),
                          "runs/s");

            std::cout << Console::TextBlue << "[ITERATIONS] "
                      << Console::TextDefault
                      << std::setprecision(3)
                      << "       Average time: "
                      << result.IterationTimeAverage() / 1000.0 << " us"
                      << std::endl;

            PAD_DEVIATION_INVERSE("Fastest: ",
                                  (result.IterationTimeMinimum() / 1000.0),
                                  (result.IterationTimeAverage() / 1000.0),
                                  "us");
            PAD_DEVIATION_INVERSE("Slowest: ",
                                  (result.IterationTimeMaximum() / 1000.0),
                                  (result.IterationTimeAverage() / 1000.0),
                                  "us");

            std::cout << std::setprecision(5);

            PAD("");
            PAD("Average performance: " <<
                result.IterationsPerSecondAverage() <<
                " iterations/s");
            PAD_DEVIATION("Best performance: ",
                          (result.IterationsPerSecondMaximum()),
                          (result.IterationsPerSecondAverage()),
                          "iterations/s");
            PAD_DEVIATION("Worst performance: ",
                          (result.IterationsPerSecondMinimum()),
                          (result.IterationsPerSecondAverage()),
                          "iterations/s");

#undef PAD_DEVIATION_INVERSE
#undef PAD_DEVIATION
#undef PAD
        }

        void DisplayDisabledTestsCount(std::size_t disabledCount)
        {
            if (disabledCount)
            {
                std::cout << std::endl << Console::TextYellow
                          << "  YOU HAVE " << disabledCount << " DISABLED "
                          << (1 == disabledCount ? "TEST" : "TESTS")
                          << Console::TextDefault << std::endl << std::endl;
            }
        }
    };
}
#endif
