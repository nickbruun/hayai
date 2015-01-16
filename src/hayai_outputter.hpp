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

        /// The total number of benchmarks registred is the sum of the two
        /// counts passed to the outputter.
        ///
        /// @param enabledCount Number of benchmarks to be executed.
        /// @param disabledCount Number of disabled benchmarks to be skipped.
        virtual void Begin(const std::size_t& enabledCount,
                           const std::size_t& disabledCount) = 0;


        /// End benchmarking.

        /// @param executedCount Number of benchmarks that have been executed.
        /// @param disabledCount Number of benchmarks that have been skipped
        /// because they are disabled.
        virtual void End(const std::size_t& executedCount,
                         const std::size_t& disabledCount) = 0;


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


        /// Skip disabled benchmark test run.

        /// @param fixtureName Fixture name.
        /// @param testName Test name.
        /// @param parameters Test parameter description.
        /// @param runsCount Number of runs to be executed.
        /// @param iterationsCount Number of iterations per run.
        virtual void SkipDisabledTest(const std::string& fixtureName,
                                      const std::string& testName,
                                      const std::string& parameters,
                                      const std::size_t& runsCount,
                                      const std::size_t& iterationsCount) = 0;


        virtual ~Outputter()
        {

        }
    };
}
#endif
