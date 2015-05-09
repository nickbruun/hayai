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
        virtual void Begin(const std::size_t& enabledCount,
                           const std::size_t& disabledCount)
        {
            std::cout << std::fixed;
            std::cout << Console::TextGreen << "[==========]"
                      << Console::TextDefault << " Running "
                      << enabledCount
                      << (enabledCount == 1 ?
                          " benchmark." :
                          " benchmarks");

            if (disabledCount)
                std::cout << ", skipping "
                          << disabledCount
                          << (disabledCount == 1 ?
                              " benchmark." :
                              " benchmarks");
            else
                std::cout << ".";

            std::cout << std::endl;
        }


        virtual void End(const std::size_t& executedCount,
                         const std::size_t& disabledCount)
        {
            std::cout << Console::TextGreen << "[==========]"
                      << Console::TextDefault << " Ran " << executedCount
                      << (executedCount == 1 ?
                          " benchmark." :
                          " benchmarks");

            if (disabledCount)
                std::cout << ", skipped "
                          << disabledCount
                          << (disabledCount == 1 ?
                              " benchmark." :
                              " benchmarks");
            else
                std::cout << ".";

            std::cout << std::endl;
        }


        inline void BeginOrSkipTest(const std::string& fixtureName,
                                    const std::string& testName,
                                    const TestParametersDescriptor& parameters,
                                    const std::size_t& runsCount,
                                    const std::size_t& iterationsCount,
                                    const bool skip)
        {
            if (skip)
                std::cout << Console::TextCyan << "[ DISABLED ]";
            else
                std::cout << Console::TextGreen << "[ RUN      ]";

            std::cout << Console::TextYellow << " ";
            WriteFormattedName(std::cout, fixtureName, testName, parameters);
            std::cout << Console::TextDefault
                      << " (" << runsCount
                      << (runsCount == 1 ? " run, " : " runs, ")
                      << iterationsCount
                      << (iterationsCount == 1 ?
                          " iteration per run)" :
                          " iterations per run)")
                      << std::endl;
        }


        virtual void BeginTest(const std::string& fixtureName,
                               const std::string& testName,
                               const TestParametersDescriptor& parameters,
                               const std::size_t& runsCount,
                               const std::size_t& iterationsCount)
        {
            BeginOrSkipTest(fixtureName,
                            testName,
                            parameters,
                            runsCount,
                            iterationsCount,
                            false);
        }


        virtual void SkipDisabledTest(
            const std::string& fixtureName,
            const std::string& testName,
            const TestParametersDescriptor& parameters,
            const std::size_t& runsCount,
            const std::size_t& iterationsCount
        )
        {
            BeginOrSkipTest(fixtureName,
                            testName,
                            parameters,
                            runsCount,
                            iterationsCount,
                            true);
        }


        virtual void EndTest(const std::string& fixtureName,
                             const std::string& testName,
                             const TestParametersDescriptor& parameters,
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
                      << Console::TextYellow << " ";
            WriteFormattedName(std::cout, fixtureName, testName, parameters);
            std::cout << Console::TextDefault << " ("
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
    private:
        inline void WriteFormattedName(
            std::ostream& stream,
            const std::string& fixtureName,
            const std::string& testName,
            const TestParametersDescriptor& parameters
        )
        {
            stream << fixtureName << "." << testName;

            const std::vector<TestParameterDescriptor>& descs =
                parameters.Parameters();

            if (descs.empty())
                return;

            stream << "(";

            for (std::size_t i = 0; i < descs.size(); ++i)
            {
                if (i)
                    stream << ", ";

                const TestParameterDescriptor& desc = descs[i];
                stream << desc.Type << " " << desc.Name << " = " << desc.Value;
            }

            stream << ")";
        }
    };
}
#endif
