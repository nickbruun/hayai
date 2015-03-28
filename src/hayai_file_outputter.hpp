#ifndef __HAYAI_FILEOUTPUTTER
#define __HAYAI_FILEOUTPUTTER
#include "hayai_outputter.hpp"
#include "hayai_console.hpp"

namespace hayai
{
    /// File outputter.

    /// Prints the result to output file.
    /// If the file can't be opened - to standard output.
    class FileOutputter
            :   public Outputter
    {
    public:
        /// @param fileName Name of the output file.
        /// @param openMode Mode with which the file will be opened.
        FileOutputter(const char* fileName,
                      std::ios_base::openmode openMode = std::ios::out)
                : _file(fileName, openMode),
                  _stream(_file ? _file : std::cout)
        {
            if(!_file)
                std::cout << "Could not open \"" 
						  << fileName << "\" file." 
						  << std::endl;
        }


        virtual void Begin(const std::size_t& enabledCount,
                           const std::size_t& disabledCount)
        {
            _stream << std::fixed;
            _stream << Console::TextGreen << "[==========]"
                    << Console::TextDefault << " Running "
                    << enabledCount
                    << (enabledCount == 1 ?
                        " benchmark." :
                        " benchmarks");

            if (disabledCount)
                _stream << ", skipping "
                        << disabledCount
                        << (disabledCount == 1 ?
                            " benchmark." :
                            " benchmarks");
            else
                _stream << ".";

            _stream << std::endl;
        }


        virtual void End(const std::size_t& executedCount,
                         const std::size_t& disabledCount)
        {
            _stream << Console::TextGreen << "[==========]"
                    << Console::TextDefault << " Ran " << executedCount
                    << (executedCount == 1 ?
                        " benchmark." :
                        " benchmarks");

            if (disabledCount)
                _stream << ", skipped "
                        << disabledCount
                        << (disabledCount == 1 ?
                            " benchmark." :
                            " benchmarks");
            else
                _stream << ".";

            _stream << std::endl;
        }


        inline void BeginOrSkipTest(const std::string& fixtureName,
                                    const std::string& testName,
                                    const std::string& parameters,
                                    const std::size_t& runsCount,
                                    const std::size_t& iterationsCount,
                                    const bool skip)
        {
            if (skip)
                _stream << Console::TextCyan << "[ DISABLED ]";
            else
                _stream << Console::TextGreen << "[ RUN      ]";

            _stream << Console::TextYellow << " "
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


        virtual void BeginTest(const std::string& fixtureName,
                               const std::string& testName,
                               const std::string& parameters,
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


        virtual void SkipDisabledTest(const std::string& fixtureName,
                                      const std::string& testName,
                                      const std::string& parameters,
                                      const std::size_t& runsCount,
                                      const std::size_t& iterationsCount)
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
                             const std::string& parameters,
                             const TestResult& result)
        {
#define PAD(x) _stream << std::setw(34) << x << std::endl;
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

            _stream << Console::TextGreen << "[     DONE ]"
                    << Console::TextYellow << " "
                    << fixtureName << "."
                    << testName
                    << parameters
                    << Console::TextDefault << " ("
                    << std::setprecision(6)
                    << (result.TimeTotal() / 1000000.0) << " ms)"
                    << std::endl;

            _stream << Console::TextBlue << "[   RUNS   ] "
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

            _stream << std::setprecision(5);

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

            _stream << Console::TextBlue << "[ITERATIONS] "
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

            _stream << std::setprecision(5);

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

        std::ofstream _file;    ///< Output file handler
        std::ostream& _stream;  ///< Output stream
    };
}
#endif
