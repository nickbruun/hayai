#ifndef __HAYAI_TEST
#define __HAYAI_TEST
#include <cstddef>

#include "hayai_clock.hpp"
#include "hayai_test_result.hpp"

namespace hayai
{
    /// Base test class.

    /// @ref SetUp is invoked before each run, and @ref TearDown is invoked
    /// once the run is finished. Iterations rely on the same fixture
    /// for every run.
    ///
    /// The default test class does not contain any actual code in the
    /// SetUp and TearDown methods, which means that tests can inherit
    /// this class directly for non-fixture based benchmarking tests.
    class Test
    {
    public:
        /// Set up the testing fixture for execution of a run.
        virtual void SetUp()
        {

        }


        /// Tear down the previously set up testing fixture after the
        /// execution run.
        virtual void TearDown()
        {

        }


        /// Run the test.

        /// @param iterations Number of iterations to gather data for.
        /// @returns the number of microseconds the run took.
        int64_t Run(std::size_t iterations)
        {
            // Set up the testing fixture.
            SetUp();

            // Get the starting time.
            Clock::TimePoint startTime, endTime;

            startTime = Clock::now();

            // Run the test body for each iteration.
            std::size_t iteration = iterations;
            while (iteration--)
                TestBody();

            // Get the ending time.
            endTime = Clock::now();

            // Tear down the testing fixture.
            TearDown();

            // Return the duration in microseconds.
            return Clock::diff(startTime, endTime);
        }


        virtual ~Test()
        {

        }
    protected:
        /// Test body.

        /// Executed for each iteration the benchmarking test is run.
        virtual void TestBody()
        {

        }
    };
}
#endif
