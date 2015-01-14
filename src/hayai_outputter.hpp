#ifndef __HAYAI_OUTPUTTER
#define __HAYAI_OUTPUTTER
#include <cstddef>
#include "hayai_test_result.hpp"

namespace hayai
{
    /// Outputter.

    /// Abstract base class for outputters.
    class Outputter
    {
    public:
        /// Begin benchmarking.

        /// @param benchmarksCount Number of benchmarks to be executed.
        virtual void Begin(const std::size_t& benchmarksCount) = 0;


        /// End benchmarking.

        /// @param benchmarksCount Number of benchmarks that have been
        /// executed.
        virtual void End(const std::size_t& benchmarksCount) = 0;

        /// Show disabled tests count.
        /// @param disabledCount Number of disabled tests
        virtual void DisplayDisabledTestsCount(std::size_t disabledCount) = 0;

        /// Begin benchmark test run.

        /// @param fixtureName Fixture name.
        /// @param testName Test name.
        /// @param parameters Test parameter description.
        /// @param runsCount Number of runs to be executed.
        /// @param iterationsCount Number of iterations per run.
        virtual void BeginTest(const std::string& fixtureName,
                               const std::string& testName,
                               const std::string& parameters,
                               const std::size_t& runsCount,
                               const std::size_t& iterationsCount) = 0;


        /// End benchmark test run.

        /// @param fixtureName Fixture name.
        /// @param testName Test name.
        /// @param parameters Test parameter description.
        /// @param result Test result.
        virtual void EndTest(const std::string& fixtureName,
                             const std::string& testName,
                             const std::string& parameters,
                             const TestResult& result) = 0;

        virtual ~Outputter()
        {

        }
    };
}
#endif
